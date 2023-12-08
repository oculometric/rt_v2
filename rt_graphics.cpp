#include "rt_graphics.h"

#include "rt_camera.h"
#include "rt_geometry.h"
#include "bmp.h"
#include "rt_rng.h"

#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

void rt_vbuf::render_pixel(const rt_vector2 & pixel, const uint32_t buffer_pos)
{
    rt_colour colour{ 1,1,1 };
    rt_ray ray;
    rt_vector2 pixel_disturbed = pixel;
    randomise_subpixel(pixel_disturbed);
    ray_for_pixel(camera, pixel_disturbed, ray);
    sample(ray, buffer_pos, 0, colour);
    add(colour_buffer[buffer_pos], colour, colour_buffer[buffer_pos]);
}

void rt_vbuf::sample(const rt_ray & ray, const uint32_t buffer_pos, const uint16_t depth, rt_colour & out)
{
    if (depth >= sample_depth) { out = background_colour; return; }

    // rt_raycast_result rcr;

    graphics_buffer->raycast(ray, rcr);

    if (depth == 0)
    {
        depth_buffer[buffer_pos] = rcr.distance;
        index_buffer[buffer_pos] = rcr.hit_tri;
        normal_buffer[buffer_pos] = rt_colour{ 0,0,0 };
    }

    if (rcr.hit_obj == NULL) { sample_sky(sky, ray.direction, out); return; }

    // perform a sample deeper into the scene
    rt_ray new_ray;
    new_ray.origin = rcr.point;

    rt_vector3 normal;
    calculate_normal(rcr.hit_obj, rcr.hit_tri, rcr.baryc, normal);
    if (depth == 0) normal_buffer[buffer_pos] = normal;
    reflect(ray.direction, normal, new_ray.direction);

    rt_material * material = rcr.hit_obj->materials[rcr.hit_obj->material_indices[rcr.hit_tri]];
    float d = 1;

    if (material->roughness > 0)
    {
        rt_vector3 perturber = rt_vector3{next_randomf(), next_randomf(), next_randomf()};
        mul(perturber, 5, perturber);
        sub(perturber, rt_vector3{1,1,1}, perturber);
        mul(perturber, material->roughness, perturber);
        add(normal, perturber, new_ray.direction);
        norm_self(new_ray.direction);
        dot(new_ray.direction, normal, d);
    }

    div(rt_vector3{1,1,1}, new_ray.direction, new_ray.direction_inverse);
    sample(new_ray, buffer_pos, depth+1, out);

    mul(out, material->colour, out);
    add(out, material->emission, out);
}

void rt_vbuf::randomise_subpixel(rt_vector2 & out)
{
    float f1 = fmod((float)next_random()/(float)UINT32_MAX, 1.0f);
    float f2 = fmod((float)next_random()/(float)UINT32_MAX, 1.0f);
    out = rt_vector2{out.u + f1, out.v + f2};
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

    cout << "initialising buffers" << endl;
    colour_buffer = new rt_colour[buffer_length];
    normal_buffer = new rt_colour[buffer_length];
    composite_buffer = new rt_colour[buffer_length];
    depth_buffer = new float[buffer_length];
    index_buffer = new uint16_t[buffer_length];

    rotl(0xF0CC5811BA115, 2);

    cout << "rendering scene" << endl;

    auto start = chrono::high_resolution_clock::now();

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

    auto end = chrono::high_resolution_clock::now();
    cout << "rendering took " << ((float)(end-start).count())/1000000.0f << endl;

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
    else if (buffer_selection == VBUF_COMPOSITE)
    {
        for (int i = 0; i < buffer_length; i++)
        {
            gamma_correct(composite_buffer[i], camera->exposure, camera->gamma, out_buffer+(i*3));
        }
    }
    else
    {
        rt_colour * in_buffer;
        if (buffer_selection == VBUF_COLOUR) in_buffer = colour_buffer;
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

    rt_material * diffuse_mat = new rt_material;
    diffuse_mat->colour = rt_colour{0.8, 0.8, 0.8};
    diffuse_mat->emission = rt_colour{0,0,0};
    diffuse_mat->roughness = 1;

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
        } else if (split_line[0] == "sample_depth")
        {
            if (split_line.size() < 2) { cout << "malformed config at line " << i << endl; return; }
            sample_depth = stoi(split_line[1]);
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
        } else if (split_line[0] == "background_colour")
        {
            if (split_line.size() < 4) { cout << "malformed config at line " << i << endl; return; }
            background_colour = rt_colour{ stof(split_line[1]), stof(split_line[2]), stof(split_line[3]) };
        }  else if (split_line[0] == "output")
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
            if (loaded_obj != NULL) { loaded_obj->materials[0] = diffuse_mat; graphics_buffer->insert_object(loaded_obj); }
        }
    }
}
