#include "rt_graphics.h"

#include "rt_camera.h"
#include "rt_geometry.h"
#include "bmp.h"

#include <iostream>
#include <fstream>
using namespace std;

void rt_vbuf::render_pixel(rt_vector2 & pixel, uint32_t buffer_pos)
{
    rt_colour colour{ 1,1,1 };
    rt_ray ray;
    rt_raycast_result rcr;
    rt_material * material;

    // initial raycast
    ray_for_pixel(camera, pixel, ray);
    graphics_buffer->raycast(ray, rcr);

    if (rcr.hit_obj != NULL)
    {
        // if intersected, reflect and recast
        colour = /*rcr.hit_tri->material->diffuse_colour*/ rt_colour{1,0,1};
        index_buffer[buffer_pos] = rcr.hit_tri;
    }
    else
    {
        // otherwise, sample the sky and terminate raycast
        sample_sky(sky, ray.direction, colour);
        depth_buffer[buffer_pos] = camera->far_clip;
        normal_buffer[buffer_pos] = ray.direction;
        index_buffer[buffer_pos] = 0;
    }

    add(colour_buffer[buffer_pos], colour, colour_buffer[buffer_pos]);
}

void rt_vbuf::render()
{
    cout << "computing camera vectors" << endl;
    compute_vectors(camera);
    buffer_length = camera->view_size_pixels.u * camera->view_size_pixels.v;

    graphics_buffer->near_clip = camera->near_clip;
    graphics_buffer->far_clip = camera->far_clip;
    
    cout << "cleaning up" << endl;
    clean_up();

    cout << "rendering scene" << endl;
    colour_buffer = new rt_colour[buffer_length];
    normal_buffer = new rt_colour[buffer_length];
    composite_buffer = new rt_colour[buffer_length];
    depth_buffer = new float[buffer_length];
    index_buffer = new uint16_t[buffer_length];

    rt_vector2 pixel = rt_vector2{ 0,0 };
    for (uint32_t p = 0; p < buffer_length; p++)
    {
        colour_buffer[p] = rt_vector3{0,0,0};
        
        for (uint32_t i = 0; i < samples_per_pixel; i++) render_pixel(pixel, p);
        div(colour_buffer[p], samples_per_pixel, colour_buffer[p]);

        pixel.u += 1;
        if (pixel.u >= camera->view_size_pixels.u)
        {
            pixel.u = 0;
            pixel.v += 1;
        }
    }

    cout << "compositing" << endl;
    composite();
    cout << "outputting buffers" << endl;
    output_buffers();
}

void rt_vbuf::composite()
{
    // TODO: real compositing. for now we just copy from the colour buffer
    for (uint32_t i = 0; i < buffer_length; i++)
    {
        composite_buffer[i] = colour_buffer[i];
    }
}

void rt_vbuf::blit(uint8_t * out_buffer, uint8_t buffer_selection)
{
    if (buffer_selection == VBUF_DEPTH)
    {
        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[i] = (uint8_t)(((depth_buffer[i]-camera->near_clip)/(camera->far_clip-camera->near_clip))*255);
        }
    }
    else if (buffer_selection == VBUF_INDEX)
    {
        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[(i*3) + 0] = (uint8_t)(index_buffer[i] & 0xFF);
            out_buffer[(i*3) + 1] = (uint8_t)((index_buffer[i] >> 8) & 0xFF);
            out_buffer[(i*3) + 2] = 0;
        }
    }
    else
    {
        rt_colour * in_buffer;
        if (buffer_selection == VBUF_COMPOSITE) in_buffer = composite_buffer;
        else if (buffer_selection == VBUF_COLOUR) in_buffer = colour_buffer;
        else if (buffer_selection == VBUF_NORMAL) in_buffer = normal_buffer;
        else return;

        for (int i = 0; i < buffer_length; i++)
        {
            out_buffer[(i*3) + 0] = (uint8_t)(in_buffer[i].x*255);
            out_buffer[(i*3) + 1] = (uint8_t)(in_buffer[i].y*255);
            out_buffer[(i*3) + 2] = (uint8_t)(in_buffer[i].z*255);
        }
    }
}

void rt_vbuf::clean_up()
{
    if (colour_buffer != NULL) { delete[] colour_buffer; colour_buffer = NULL; }
    if (normal_buffer != NULL) { delete[] normal_buffer; normal_buffer = NULL; }
    if (depth_buffer != NULL) { delete[] depth_buffer; depth_buffer = NULL; }
    if (composite_buffer != NULL) { delete[] composite_buffer; composite_buffer = NULL; }
    if (index_buffer != NULL) { delete[] index_buffer; index_buffer = NULL; }
}

void rt_vbuf::output_buffers()
{
    uint8_t * output_buffer = new uint8_t[buffer_length*3];
    
    if (buffers_to_output & (0b1 << VBUF_COLOUR))
    {
        blit(output_buffer, VBUF_COLOUR);
        write_bmp(output_buffer, camera->view_size_pixels.u, camera->view_size_pixels.v, 3, "colour.bmp");
    }
    if (buffers_to_output & (0b1 << VBUF_COMPOSITE))
    {
        blit(output_buffer, VBUF_COMPOSITE);
        write_bmp(output_buffer, camera->view_size_pixels.u, camera->view_size_pixels.v, 3, "composite.bmp");
    }
    if (buffers_to_output & (0b1 << VBUF_NORMAL))
    {
        blit(output_buffer, VBUF_NORMAL);
        write_bmp(output_buffer, camera->view_size_pixels.u, camera->view_size_pixels.v, 3, "normal.bmp");
    }
    if (buffers_to_output & (0b1 << VBUF_DEPTH))
    {
        blit(output_buffer, VBUF_DEPTH);
        write_bmp(output_buffer, camera->view_size_pixels.u, camera->view_size_pixels.v, 1, "depth.bmp");
    }
    if (buffers_to_output & (0b1 << VBUF_INDEX))
    {
        blit(output_buffer, VBUF_INDEX);
        write_bmp(output_buffer, camera->view_size_pixels.u, camera->view_size_pixels.v, 3, "index.bmp");
    }

    delete[] output_buffer;
}

rt_vbuf::rt_vbuf()
{
    camera = new rt_camera;
    sky = new rt_simplesky;
    sun = new rt_sun;
    graphics_buffer = new rt_gbuf;
    buffers_to_output = 0b0;
}

rt_vbuf::rt_vbuf(const char * path)
{
    camera = new rt_camera;
    sky = new rt_simplesky;
    sun = new rt_sun;
    graphics_buffer = new rt_gbuf;
    buffers_to_output = 0b0;

    ifstream file;
    file.open(path);
    if (!file.is_open()) { cout << "failed to open config file" << endl; return; }

    cout << "attempting to read config file" << endl;

    string line;
    vector<string> split_line;
    int i = 0;
    while (getline(file, line))
    {
        i++;
        if (line.length() < 1) continue;
        if (line[0] == '#') continue;
        split(line, split_line, ' ');
        if (split_line.size() == 0) { cout << "ignoring blank line " << i << endl; continue;}
        if (split_line[0] == "camera_position")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            camera->position = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        } else if (split_line[0] == "camera_direction")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            camera->look_direction = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
            norm_self(camera->look_direction);
        } else if (split_line[0] == "camera_fov")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->field_of_view = stof(split_line[1]);
        } else if (split_line[0] == "camera_pixels")
        {
            if (split_line.size() < 3) { cout << "malformed config at line " << i << endl; return; }
            camera->view_size_pixels = rt_vector2{ (float)stoi(split_line[1]), (float)stoi(split_line[2]) };
        } else if (split_line[0] == "camera_nearclip")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->near_clip = stof(split_line[1]);
        } else if (split_line[0] == "camera_farclip")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->far_clip = stof(split_line[1]);
        } else if (split_line[0] == "camera_fogstart")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->fog_start = stof(split_line[1]);
        } else if (split_line[0] == "camera_fogend")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->fog_end = stof(split_line[1]);
        } else if (split_line[0] == "camera_exposure")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->exposure = stof(split_line[1]);
        } else if (split_line[0] == "camera_gamma")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            camera->gamma = stof(split_line[1]);
        } else if (split_line[0] == "sky_upper")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            sky->upper = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        } else if (split_line[0] == "sky_horizon")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            sky->horizon = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        } else if (split_line[0] == "sky_lower")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            sky->lower = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        } else if (split_line[0] == "sun_colour")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            sun->colour = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        } else if (split_line[0] == "sun_direction")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            sun->direction = rt_vector3{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
            norm_self(sun->direction);
        }  else if (split_line[0] == "sun_angle")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            sun->cos_angle = cosf(stof(split_line[1]));
        } else if (split_line[0] == "samples")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            samples_per_pixel = stoi(split_line[1]);
        } else if (split_line[0] == "dithering_mode")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            dithering_mode = stoi(split_line[1]);
            if (dithering_mode < 0 || dithering_mode > 2) { dithering_mode = 0; cout << "unsupported dithering mode " << dithering_mode << endl; return; }
        } else if (split_line[0] == "view_transform")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            view_transform = stoi(split_line[1]);
            if (view_transform < 0 || view_transform > 2) { view_transform = 0; cout << "unsupported view transform " << view_transform << endl; return; }
        } else if (split_line[0] == "backface_culling")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            graphics_buffer->cull_backfaces = stoi(split_line[1]);
        } else if (split_line[0] == "output")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            int pass = stoi(split_line[1]);
            buffers_to_output |= (0b1 << pass);
        } else if (split_line[0] == "input")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            string filename_to_load = line.substr(6, line.find('#'));
            if (filename_to_load.length() == 0) { cout << "malformed config at line " << i << endl; return; }
            cout << "reading object file " << filename_to_load << endl;
            rt_object * loaded_obj = load_obj_file(filename_to_load.c_str());
            if (loaded_obj != NULL) graphics_buffer->insert_object(loaded_obj);
        }
    }
}
