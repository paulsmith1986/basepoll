#include "first_protocol_cpp.h"

/**
 * 写入string
 */
void proto_write_string( protocol_result_t *result, std::string &tmp_str )
{
	size_t len = tmp_str.size();
	if ( len > 65535 )
	{
		throw PROTO_ERROR_TOOBIG_DATA;
	}
	string_len_t str_len = len;
	first_result_push_data( result, &str_len, sizeof( string_len_t ) );
	first_result_push_data( result, (void*)tmp_str.c_str(), str_len );
}