#ifndef KURO_XML_PARSE_SCENE_H
#define KURO_XML_PARSE_SCENE_H

#include "data-structures.h"
#include "xml/xml-data-structures.h"
#include "xml/xml_tag.h"
#include "xml/xml_tokeniser_basic.h"
#include "xml/xml_tokeniser_tags.h"

namespace kuro
{
    namespace xml
    {
        namespace parse
        {
            /*parses a set of tags into a scene data structure for rendering*/
            namespace scene
            {
                /*searches a list of attributes and returns the first attribute with a
                matching name*/
                std::string get_attribute_value(tag::tag* tag, std::string attribute_name);

                /*searches a list of child tags and returns the child tag
                with a matching name*/
                tag::tag* get_child_tag(tag::tag* tag, std::string name);

                /*parses a set of tags into a scene data structure*/
                kuro::scene::scene* parse(tag::tag* tag);

                /*parses an object tag into an object data structure*/
                kuro::object::base* parse_object(tag::tag* tag);

                /*parses an object tag into a sphere object data structure*/
                kuro::object::sphere* parse_sphere(tag::tag* tag);

                /*parses an object tag into a mesh object data structure*/
                kuro::object::mesh* parse_mesh(tag::tag* tag);

                /*parses a bvh cache tag and returns the filename of the bvh*/
                std::string parse_bvh_cache(tag::tag* tag);

                /*parses a BRDF tag into a BRDF data structure*/
                kuro::brdf::brdf* parse_brdf(tag::tag* tag);

                /*parses a BRDF tag of type 'plain-colour' into a plain BRDF data structure*/
                kuro::brdf::basic* parse_brdf_plain(tag::tag* tag);

                /*parses a BRDF tag of type 'ambient-occlusion' into a ambient occlusion BRDF data structure*/
                kuro::brdf::ambient_occ* parse_brdf_ambient_occ(tag::tag* tag);

                /*parses a BRDF tag of type 'simple-reflect' into a simple reflect BRDF data structure*/
                kuro::brdf::simple_reflect* parse_brdf_simple_reflect(tag::tag* tag);

                /*parses a BRDF tag of type 'emission' into a emission BRDF data structure*/
                kuro::brdf::emission* parse_brdf_emission(tag::tag* tag);

                /*parses a BRDF tag of type 'diffuse' into a diffuse BRDF data structure*/
                kuro::brdf::diffuse* parse_brdf_diffuse(tag::tag* tag);

				/*parses a BRDF tag of type 'normal-diffuse' into a normal diffuse BRDF structure*/
				kuro::brdf::normal_diffuse* parse_brdf_normal_diffuse(tag::tag* tag);

				/*parses a BRDF tag of type 'mix' into a mix BRDF structure*/
				kuro::brdf::mix* parse_brdf_mix(tag::tag* tag);

                /*parses an RGBA tag into a RGBA data structure*/
                kuro::util::rgba parse_rgba(tag::tag* tag);

                /*parses a camera tag into a camera data structure*/
                kuro::camera::base* parse_camera(tag::tag* tag);

				/*parses a basic camera tag into a camera data structure*/
                kuro::camera::camera* parse_basic_camera(tag::tag* tag);

				/*parses a lens camera tag into a camera data structure*/
                kuro::camera::lens_camera* parse_lens_camera(tag::tag* tag);

                /*parses a background tag into a BRDF data structure*/
                kuro::brdf::brdf* parse_background_brdf(tag::tag* tag);

                /*parses a settings tag into a render settings data structure*/
                kuro::scene::render_settings parse_settings(tag::tag* tag);

                /*converts a string into a floating point or double data structure*/
                kuro::util::real parse_real(std::string value);

                /*converts a string into an integer*/
                int parse_integer(std::string value);

                /*parses a transformation tag*/
                kuro::math::vec4f parse_transform(tag::tag* tag);
        }
        }
    }
}

#endif // KURO_XML_PARSE_SCENE_H
