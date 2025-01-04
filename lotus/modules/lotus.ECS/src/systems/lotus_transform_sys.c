#include "../../include/systems/lotus_transform_sys.h"

#include "../../include/components/lotus_component.h"

ubyte _lotus_transform_system(void* manager, Lotus_Entity entity) {
    if (
        entity > LOTUS_ENTITY_MAX || entity < 0 ||
        !((Lotus_Component_Manager*)manager)->component_register[LOTUS_TRANSFORM_COMPONENT].active[entity]
        ) {
        return LOTUS_FALSE;
    }
    
    // translation -> rotation -> scaling
    Lotus_Transform_Data* data = (Lotus_Transform_Data*)((Lotus_Component_Manager*)manager)->component_register[LOTUS_TRANSFORM_COMPONENT].data;
    Lotus_Mat4 model = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(
        data->location[entity].x,
        data->location[entity].y,
        data->location[entity].z
    ));
    
    model = lotus_mul_mat4(model, lotus_rot_mat4(
        1, 0, 0, data->rotation[entity].x
    ));
    model = lotus_mul_mat4(model, lotus_rot_mat4(
        0, 1, 0, data->rotation[entity].y
    ));
    model = lotus_mul_mat4(model, lotus_rot_mat4(
        0, 0, 1, data->rotation[entity].z
    ));
    
    model = lotus_mul_mat4(model, lotus_scale_mat4(
        data->scale[entity].x, data->scale[entity].y, data->scale[entity].z
    ));

    data->model[entity] = model;

    return LOTUS_TRUE;
}
