#ifndef KURO_XML_PARSE_SCENE_CPP
#define KURO_XML_PARSE_SCENE_CPP

#include <string>
#include <math.h>
#include "data-structures.h"
#include "util.h"
#include "brdf.h"
#include "object.h"
#include "mesh.h"
#include "binary_tree.h"
#include "bmp.h"
#include "camera.h"
#include "scene.h"
#include "xml_parse_scene.h"

namespace kuro
{
    namespace xml
    {
        namespace parse
        {
            namespace scene
            {
                std::string get_attribute_value(tag::tag* tag, std::string attribute_name)
                {
                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        if(tag->attribute_list[i].name == attribute_name)
                        {
                            return tag->attribute_list[i].value;
                        }
                    }

                    return "";
                }

                tag::tag* get_child_tag(tag::tag* tag, std::string name)
                {
                    tag = tag->child;

                    for(tag::tag* next = tag;next != NULL;next = next->next)
                    {
                        if(next->name == name)
                        {
                            return next;
                        }
                    }

                    return NULL;
                }

                kuro::scene::scene* parse(tag::tag* tag)
                {
                    kuro::scene::scene* scn = new kuro::scene::scene;
                    scn->object = NULL;
                    scn->camera = NULL;
                    scn->background_brdf = NULL;

                    kuro::object::base* previous = NULL;
                    kuro::object::base* current = NULL;

                    for(tag::tag* next = tag;next != NULL;next = next->next)
                    {
                        if(next->name == "object")
                        {
                            std::cout<<"new object"<<std::endl;

                            current = parse_object(next);

                            if(current == NULL)
                            {
                                std::cout<< "error parsing object." <<std::endl;
                                continue;
                            }

                            if(previous == NULL)
                            {
                                previous = current;
                                previous->next = NULL;
                            }
                            else
                            {
                                previous->next = current;
                                previous = previous->next;

                                previous->next = NULL;
                            }

                            if(scn->object == NULL)
                            {
                                scn->object = previous;
                            }
                        }

                        if(next->name == "camera")
                        {
                            scn->camera = parse_camera(next);
                        }

                        if(next->name == "background")
                        {
                            scn->background_brdf = parse_background_brdf(next);
                        }

                        if(next->name == "settings")
                        {
                            scn->settings = parse_settings(next);
                        }
                    }

                    return scn;
                }

                kuro::object::base* parse_object(tag::tag* tag)
                {
                    std::string type = get_attribute_value(tag, "type");
                    kuro::object::base* object = NULL;

                    if(type == "sphere")
                    {
                        object = parse_sphere(tag);
                    }

                    if(type == "mesh")
                    {
                        object = parse_mesh(tag);
                    }

                    if(object != NULL)
                    {
                        tag::tag* brdf_tag = get_child_tag(tag, "bsdf");

                        if(brdf_tag != NULL)object->object_brdf = parse_brdf(brdf_tag);
                    }

                    return object;
                }

                kuro::object::sphere* parse_sphere(tag::tag* tag)
                {
                    kuro::object::sphere* sphere = new kuro::object::sphere;

                    sphere->p = vec4f(0.0, 0.0, 0.0);
                    sphere->r = 1.0;

                    string name = "", value = "";

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "X")
                        {
                            std::cout<< "X: " << value <<std::endl;
                            sphere->p.x = parse_real(value);
                        }

                        if(name == "Y")
                        {
                            std::cout<< "Y: " << value <<std::endl;
                            sphere->p.y = parse_real(value);
                        }

                        if(name == "Z")
                        {
                            std::cout<< "Z: " << value <<std::endl;
                            sphere->p.z = parse_real(value);
                        }

                        if(name == "R")
                        {
                            std::cout<< "R: " << value <<std::endl;
                            sphere->r = parse_real(value);
                        }
                    }

                    return sphere;
                }

                kuro::object::mesh* parse_mesh(tag::tag* tag)
                {
                    kuro::object::btree_mesh* mesh = new kuro::object::btree_mesh;

                    string name = "", value = "";

                    string src = "";

                    string bvh_cache_src = "";
                    bool load_bvh = false;
                    bool max_depth_set = false;

                    mesh->max_depth = 0;

                    mesh->transform_matrix = mat4f_identity;
					mesh->inv_transform_matrix = mat4f_identity;
					mesh->smooth_shading = false;

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "src")
                        {
                            std::cout<< "src: " << value <<std::endl;
                            mesh->import(value);

                            if(mesh->triangles.size() == 0)
                            {
                                std::cout<< "warning: mesh not successfully parsed." <<std::endl;
                            }
                        }

                        if(name == "max_depth")
                        {
                            std::cout<< "max_depth: " << value <<std::endl;
                            mesh->max_depth = parse_integer(value);

                            max_depth_set = true;
                        }

                        if(name == "smoothing")
                        {
                            std::cout<< "smoothing: " << value <<std::endl;
                            if(value=="true")mesh->smooth_shading = true;
                        }
                    }

                    for(tag::tag* next = tag->child;next != NULL;next = next->next)
                    {
                        name = next->name;

                        if(name == "translate")
                        {
                            std::cout<<"translate"<<endl;
                            vec4f delta = parse_transform(next);
                            //mesh->translate(vec4f()-delta);
							mesh->translate(delta);
                        }

                        if(name == "rotate")
                        {
                            std::cout<<"rotate"<<endl;

                            vec4f axis = parse_transform(next);
                            kuro::util::real theta = axis.w;
                            axis.w = 0.0f;

                            //mesh->rotate(-1.0*axis, theta);
							mesh->rotate(axis, theta);
                        }

						if(name == "scale")
						{
							std::cout<<"scale"<<endl;
                            vec4f sf = parse_transform(next);

                            //sf.x = 1.0f/sf.x;
                            //sf.y = 1.0f/sf.y;
                            //sf.z = 1.0f/sf.z;

							mesh->scale(sf);
						}

						if(name == "bvh_cache")
						{
                            std::cout<<"bvh_cache"<<endl;

                            bvh_cache_src = parse_bvh_cache(next);
                            load_bvh = true;
						}
                    }

                    if(mesh->vnormals.size() == 0)mesh->smooth_shading = false;

                    //mesh->apply_transformation();

                    if(!max_depth_set)
                    {
                        if(mesh->triangles.size() > 1)mesh->max_depth = 2.0 * (log(((double)mesh->triangles.size())/4.0)/log(2.0));
                    }

                    bool bvh_loaded = false;

                    if(load_bvh)
                    {
                        std::cout<< "loading bvh cache src='" << bvh_cache_src << "'"  <<endl;
                    }

                    if(bvh_cache_src != "")
                    {
                        std::cout<< "loading mesh bvh cache." <<endl;
                        bvh_loaded = mesh->load_binary_tree(bvh_cache_src);
                    }

                    if(!bvh_loaded)
                    {
                        std::cout<<"building bvh from scratch."<<endl;

                        mesh->init_binary_tree();
                        mesh->partition();
                    }
                    else
                    {
                        std::cout<<"bvh cache file loaded." <<endl;
                    }

                    if((!bvh_loaded)&&load_bvh)
                    {
                        std::cout<< "saving bvh to cache." <<endl;
                        mesh->save_binary_tree(bvh_cache_src);
                    }

                    return mesh;
                }

                std::string parse_bvh_cache(tag::tag* tag)
                {
                    return get_attribute_value(tag, "src");
                }


                kuro::brdf::brdf* parse_brdf(tag::tag* tag)
                {
                    std::string type = get_attribute_value(tag, "type");
                    kuro::brdf::brdf* brdf = NULL;

                    if(type == "plain-colour")
                    {
                        brdf = parse_brdf_plain(tag);
                    }

                    if(type == "simple-reflect")
                    {
                        brdf = parse_brdf_simple_reflect(tag);
                    }

                    if(type == "ambient-occlusion")
                    {
                        brdf = parse_brdf_ambient_occ(tag);
                    }

                    if(type == "emission")
                    {
                        brdf = parse_brdf_emission(tag);
                    }

                    if(type == "diffuse")
                    {
                        brdf = parse_brdf_diffuse(tag);
                    }

					if(type == "normal-diffuse")
					{
						brdf = parse_brdf_normal_diffuse(tag);
					}

					if(type == "mix")
					{
						brdf = parse_brdf_mix(tag);
					}

                    return brdf;
                }

                kuro::brdf::basic* parse_brdf_plain(tag::tag* tag)
                {
                    kuro::brdf::basic* brdf = new kuro::brdf::basic;

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
                }

                kuro::brdf::ambient_occ* parse_brdf_ambient_occ(tag::tag* tag)
                {
					kuro::brdf::ambient_occ* brdf = new kuro::brdf::ambient_occ;

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
                }

                kuro::brdf::simple_reflect* parse_brdf_simple_reflect(tag::tag* tag)
                {
                    kuro::brdf::simple_reflect* brdf = new kuro::brdf::simple_reflect;

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
                }

                kuro::brdf::emission* parse_brdf_emission(tag::tag* tag)
                {
                    kuro::brdf::emission* brdf = new kuro::brdf::emission;

                    brdf->strength = parse_real(get_attribute_value(tag, "strength"));

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
                }

                kuro::brdf::diffuse* parse_brdf_diffuse(tag::tag* tag)
                {
                    kuro::brdf::diffuse* brdf = new kuro::brdf::diffuse;

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
                }

				kuro::brdf::normal_diffuse* parse_brdf_normal_diffuse(tag::tag* tag)
				{
					kuro::brdf::normal_diffuse* brdf = new kuro::brdf::normal_diffuse;

                    tag::tag* rgba_tag = get_child_tag(tag, "colour");

                    if(rgba_tag != NULL) brdf->colour = parse_rgba(rgba_tag);

                    return brdf;
				}

				kuro::brdf::mix* parse_brdf_mix(tag::tag* tag)
				{
					kuro::brdf::mix* brdf = new kuro::brdf::mix;
					brdf->factor = 0;
					brdf->factor = parse_real(get_attribute_value(tag, "factor"));
					brdf->brdf1 = NULL;
					brdf->brdf2 = NULL;

					bool brdf1_set = false;

					std::string name = "";

					for(tag::tag* next = tag->child;next != NULL;next = next->next)
					{
						name = next->name;

						if(next->name == "bsdf")
						{
							if(!brdf1_set)
							{
								brdf->brdf1 = parse_brdf(next);
								brdf1_set = true;
							}
							else
							{
								brdf->brdf2 = parse_brdf(next);
							}
						}
					}

					std::cout<< "mix brdf"<<std::endl;
					std::cout<< "factor="<<brdf->factor<<std::endl;

					return brdf;
				}

                kuro::util::rgba parse_rgba(tag::tag* tag)
                {
                    kuro::util::rgba rgba = kuro::util::rgba();

                    string name = "", value = "";

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "R")
                        {
                            std::cout<< "R: " << value <<std::endl;
                            rgba.r = parse_real(value);
                        }

                        if(name == "G")
                        {
                            std::cout<< "G: " << value <<std::endl;
                            rgba.g = parse_real(value);
                        }

                        if(name == "B")
                        {
                            std::cout<< "B: " << value <<std::endl;
                            rgba.b = parse_real(value);
                        }

                        if(name == "A")
                        {
                            std::cout<< "A: " << value <<std::endl;
                            rgba.a = parse_real(value);
                        }
                    }

                    return rgba;
                }

                kuro::camera::base* parse_camera(tag::tag* tag)
                {
					kuro::camera::base* camera = NULL;

					if(get_attribute_value(tag, "type") == "basic")
					{
						camera = parse_basic_camera(tag);
					}
					else if(get_attribute_value(tag, "type") == "lens")
					{
						camera = parse_lens_camera(tag);
					}
					else
					{
						camera = parse_basic_camera(tag);
					}

					string name = "";

					for(tag::tag* next = tag->child;next != NULL;next = next->next)
                    {
                        name = next->name;

                        if(name == "translate")
                        {
                            std::cout<<"translate"<<endl;
                            vec4f delta = parse_transform(next);
                            camera->translate(delta);
                        }

                        if(name == "rotate")
                        {
                            std::cout<<"rotate"<<endl;

                            vec4f axis = parse_transform(next);
                            kuro::util::real theta = axis.w;
                            axis.w = 0.0f;

                            camera->rotate(axis, theta);
                        }
                    }

                    return camera;
                }

                kuro::camera::camera* parse_basic_camera(tag::tag* tag)
				{
					kuro::camera::camera* camera = new kuro::camera::camera;
                    camera->fov_x = 90.0;
                    camera->fov_y = 90.0;
                    camera->p = vec4f();

                    camera->axis[0] = vec4f(1.0);
                    camera->axis[1] = vec4f(0.0, 1.0);
                    camera->axis[2] = vec4f(0.0, 0.0, 1.0);

                    string name = "", value = "";

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "X")
                        {
                            std::cout<< "X: " << value <<std::endl;
                            camera->p.x = parse_real(value);
                        }

                        if(name == "Y")
                        {
                            std::cout<< "Y: " << value <<std::endl;
                            camera->p.y = parse_real(value);
                        }

                        if(name == "Z")
                        {
                            std::cout<< "Z: " << value <<std::endl;
                            camera->p.z = parse_real(value);
                        }

                        if(name == "fov_X")
                        {
                            std::cout<< "fov_X: " << value <<std::endl;
                            camera->fov_x = parse_real(value);
                        }

                        if(name == "fov_Y")
                        {
                            std::cout<< "fov_Y: " << value <<std::endl;
                            camera->fov_y = parse_real(value);
                        }
                    }

                    if(camera->fov_x < 0)
                    {
                        camera->fov_x = 90.0;
                        std::cout<< "warning: negative camera fov_x value specified." <<endl;
                    }

                    if(camera->fov_y < 0)
                    {
                        camera->fov_y = 90.0;
                        std::cout<< "warning: negative camera fov_y value specified." <<endl;
                    }

                    return camera;
				}

                kuro::camera::lens_camera* parse_lens_camera(tag::tag* tag)
				{
					kuro::camera::lens_camera* camera = new kuro::camera::lens_camera;
                    camera->fov_x = 90.0;
                    camera->fov_y = 90.0;
                    camera->p = vec4f();

                    camera->axis[0] = vec4f(1.0);
                    camera->axis[1] = vec4f(0.0, 1.0);
                    camera->axis[2] = vec4f(0.0, 0.0, 1.0);

					camera->radius = 0.1;
					camera->focal_length = 0.5;
					camera->lens_distance = 0.1;
					camera->s_width = 0.2;
					camera->s_height = 0.2;

                    string name = "", value = "";

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "X")
                        {
                            std::cout<< "X: " << value <<std::endl;
                            camera->p.x = parse_real(value);
                        }

                        if(name == "Y")
                        {
                            std::cout<< "Y: " << value <<std::endl;
                            camera->p.y = parse_real(value);
                        }

                        if(name == "Z")
                        {
                            std::cout<< "Z: " << value <<std::endl;
                            camera->p.z = parse_real(value);
                        }

                        if(name == "fov_X")
                        {
                            std::cout<< "fov_X: " << value <<std::endl;
                            camera->fov_x = parse_real(value);
                        }

                        if(name == "fov_Y")
                        {
                            std::cout<< "fov_Y: " << value <<std::endl;
                            camera->fov_y = parse_real(value);
                        }

						if(name == "radius")
                        {
                            std::cout<< "radius: " << value <<std::endl;
                            camera->radius = parse_real(value);
                        }

						if(name == "focal_length")
                        {
                            std::cout<< "focal_length: " << value <<std::endl;
                            camera->focal_length = parse_real(value);
                        }

						if(name == "lens_distance")
                        {
                            std::cout<< "lens_distance: " << value <<std::endl;
                            camera->lens_distance = parse_real(value);
                        }

						if(name == "sensor_width")
                        {
                            std::cout<< "sensor_width: " << value <<std::endl;
                            camera->s_width = parse_real(value);
                        }

						if(name == "sensor_height")
                        {
                            std::cout<< "sensor_height: " << value <<std::endl;
                            camera->s_height = parse_real(value);
                        }
                    }

                    return camera;
				}


                kuro::brdf::brdf* parse_background_brdf(tag::tag* tag)
                {
                    tag::tag* brdf_tag = get_child_tag(tag, "bsdf");

                    if(brdf_tag != NULL)return parse_brdf(brdf_tag);

                    return NULL;
                }

                kuro::scene::render_settings parse_settings(tag::tag* tag)
                {
                    kuro::scene::render_settings settings;
                    settings.samples = 1;
                    settings.threads = 1;
                    settings.width = 512;
                    settings.height = 512;
                    settings.name = "camera-output.bmp";

                    string name = "";
                    string value = "";
                    int number = 1;

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "samples")
                        {
                            std::cout<< "samples: "<<value<<std::endl;
                            number = parse_integer(value);

                            if(number < 1)
                            {
                                std::cout<< "error: samples value set to 1" <<std::endl;
                                number = 1;
                            }

                            settings.samples = number;
                        }

                        if(name == "threads")
                        {
                            std::cout<< "threads: "<<value<<std::endl;
                            number = parse_integer(value);

                           if(number < 1)
                            {
                                std::cout<< "error: threads value set to 1" <<std::endl;
                                number = 1;
                            }

                            settings.threads = number;
                        }
                    }

                    tag::tag* img_settings_tag = get_child_tag(tag, "image_output");

                    if(img_settings_tag == NULL)return settings;

                    for(int i = 0;i<img_settings_tag->attribute_list.size();i++)
                    {
                        name = img_settings_tag->attribute_list[i].name;
                        value = img_settings_tag->attribute_list[i].value;

                        if(name == "name")
                        {
                            std::cout<< "name: "<<value<<std::endl;
                            settings.name = value;
                        }

                        if(name == "width")
                        {
                            std::cout<< "width: "<<value<<std::endl;
                            number = parse_integer(value);

                            if(number < 1)
                            {
                                std::cout<< "error: width value set to 512" <<std::endl;
                                number = 512;
                            }

                            settings.width = number;
                        }

                        if(name == "height")
                        {
                            std::cout<< "height: "<<value<<std::endl;
                            number = parse_integer(value);

                            if(number < 1)
                            {
                                std::cout<< "error: height value set to 512" <<std::endl;
                                number = 512;
                            }

                            settings.height = number;
                        }
                    }

                    return settings;
                }

                kuro::util::real parse_real(string value)
                {
                    return atof(value.c_str());
                }

                int parse_integer(string value)
                {
                    return atoi(value.c_str());
                }

                kuro::math::vec4f parse_transform(tag::tag* tag)
                {
                    kuro::math::vec4f v;

					if(tag->name == "scale") v = vec4f(1.0, 1.0, 1.0);

                    std::cout<< "transform."<<endl;

                    string name = "", value = "";

                    for(int i = 0;i<tag->attribute_list.size();i++)
                    {
                        name = tag->attribute_list[i].name;
                        value = tag->attribute_list[i].value;

                        if(name == "X")
                        {
                            std::cout<< "X: " << value <<std::endl;
                            v.x = parse_real(value);
                        }

                        if(name == "Y")
                        {
                            std::cout<< "Y: " << value <<std::endl;
                            v.y = parse_real(value);
                        }

                        if(name == "Z")
                        {
                            std::cout<< "Z: " << value <<std::endl;
                            v.z = parse_real(value);
                        }

                        if(name == "theta")
                        {
                            std::cout<< "W: " << value <<std::endl;
                            v.w = parse_real(value);
                        }

						if(name == "sf")
						{
							std::cout<< "sf: " << value <<std::endl;
							v = v * parse_real(value);
						}
                    }

                    return v;
                }
            }
        }
    }
}

#endif // KURO_XML_PARSE_SCENE_H
