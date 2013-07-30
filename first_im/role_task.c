#include "task.h"

/**
 * pack_id: 10011 场景移动
 */
void request_im_scene_move( fd_struct_t *fd_info, proto_im_scene_move_t *req_pack )
{
	get_role_info_from_fd_info( fd_info );
	scene_t *scene_map = find_scene( role_info->scene_pack );
	if ( NULL == scene_map )
	{
		return;
	}
	proto_im_scene_move_re_t move_out;
	move_out.role_id = role_info->role_id;
	move_out.coord = req_pack->coord;
	encode_im_scene_move_re( send_pack, &move_out );
	send_scene_data( scene_map, &send_pack );
}