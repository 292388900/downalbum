
extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
	#include "rc4.h"
	extern int luaopen_bit(lua_State *L);
}

#include "register.h"
#include "AntiDebug.h"
#include <windows.h>
#include <process.h>

// lua加密代码，源代码见文件末尾
unsigned char lua_bincode[] = {
	0x76, 0xbe, 0xed, 0x2b, 0xae, 0x43, 0xc3, 0xbe, 0x1b, 0x2c, 0x16, 0x0c, 0xab, 0x03, 0x48, 0xa5, 
	0xd1, 0x03, 0x6d, 0x5d, 0x6e, 0x40, 0xa9, 0xd4, 0x9f, 0xb5, 0x73, 0x84, 0xeb, 0x3f, 0x49, 0x7f, 
	0x37, 0x88, 0x39, 0x64, 0xcd, 0x02, 0xf5, 0xe6, 0x55, 0x28, 0x0f, 0x5c, 0xf9, 0xd3, 0x60, 0x40, 
	0x80, 0xc6, 0x60, 0x29, 0xc9, 0x9c, 0x64, 0x3d, 0xa1, 0x56, 0x47, 0x03, 0x34, 0x83, 0xf2, 0xcf, 
	0x04, 0x50, 0x5f, 0x20, 0x1b, 0x51, 0x09, 0xea, 0x1b, 0x60, 0x43, 0xfd, 0x67, 0x3a, 0x6a, 0x05, 
	0x05, 0x13, 0xa7, 0xe5, 0x2c, 0x26, 0x1e, 0x96, 0x2b, 0xb1, 0xca, 0xb4, 0x54, 0x37, 0xd9, 0xad, 
	0x6e, 0x45, 0x98, 0x50, 0x1a, 0x22, 0xe9, 0x12, 0x51, 0xd2, 0xfb, 0xfd, 0x50, 0xde, 0xcd, 0xa7, 
	0xc9, 0x4c, 0xe3, 0x4a, 0x7a, 0x7b, 0x02, 0x36, 0xf7, 0x85, 0x06, 0x1b, 0xbc, 0x08, 0xfd, 0xee, 
	0x8e, 0x6c, 0x75, 0xdb, 0xaa, 0x98, 0x4e, 0x82, 0x33, 0xaa, 0x43, 0x04, 0xbf, 0x2b, 0xaf, 0xc4, 
	0x24, 0xa6, 0x0c, 0xab, 0xe5, 0x42, 0x9a, 0x6d, 0x89, 0x0c, 0xb3, 0x67, 0xb0, 0x4b, 0x09, 0xb9, 
	0x66, 0x57, 0x4d, 0xd4, 0x7c, 0xc5, 0xc8, 0x1e, 0x48, 0x73, 0x17, 0x68, 0x7b, 0xdf, 0x0a, 0xfd, 
	0x48, 0x36, 0xcd, 0x72, 0x4f, 0x16, 0x71, 0xa0, 0x29, 0x20, 0x26, 0x2f, 0x43, 0xb8, 0xad, 0xb3, 
	0x39, 0xc6, 0x78, 0x63, 0x51, 0x54, 0xc0, 0x78, 0xee, 0x00, 0x0e, 0x46, 0xa2, 0xad, 0xed, 0xed, 
	0x79, 0x8f, 0xf4, 0xdf, 0x7b, 0x37, 0x6e, 0xd9, 0x38, 0x0e, 0xee, 0xb1, 0xe8, 0x29, 0x27, 0xb8, 
	0x39, 0x3f, 0x7d, 0x8f, 0x7b, 0xbd, 0xca, 0x9b, 0xce, 0x0f, 0x92, 0x1d, 0xdb, 0x6f, 0xf6, 0xdc, 
	0xc3, 0xe8, 0xb7, 0xbe, 0xc1, 0x82, 0xeb, 0x99, 0x3d, 0xac, 0x13, 0x07, 0xd2, 0xde, 0x2b, 0xbc, 
	0x47, 0x29, 0xf8, 0x91, 0xaa, 0x81, 0xb3, 0x59, 0xbf, 0xcd, 0x13, 0x99, 0x0d, 0x38, 0xf0, 0x68, 
	0x45, 0x7b, 0xad, 0x25, 0x5a, 0x56, 0x2b, 0x28, 0xc6, 0x00, 0xe5, 0x3e, 0x90, 0x8d, 0x4a, 0xbb, 
	0x80, 0x37, 0x79, 0xdf, 0x1e, 0x14, 0x4b, 0x55, 0x50, 0xce, 0x16, 0x87, 0x33, 0x07, 0xc6, 0x71, 
	0x76, 0xd0, 0x24, 0x44, 0x09, 0xee, 0x4a, 0xec, 0xbb, 0xe5, 0x07, 0x3e, 0xed, 0xae, 0xa4, 0x98, 
	0x1d, 0x1b, 0x40, 0xe4, 0x08, 0xb9, 0x04, 0x4d, 0x19, 0x62, 0xa9, 0x80, 0xb7, 0xb8, 0x4e, 0x11, 
	0x5b, 0x77, 0x07, 0x6c, 0x55, 0xe0, 0xc8, 0xab, 0x9f, 0xf5, 0x14, 0xc8, 0x60, 0x0c, 0xa9, 0x31, 
	0x56, 0xd2, 0x2b, 0x72, 0xe3, 0xe2, 0x42, 0x68, 0x23, 0xa6, 0x5a, 0x5a, 0x18, 0x91, 0x31, 0xd1, 
	0xa2, 0xb9, 0x85, 0xa8, 0xf4, 0xd0, 0xf8, 0x90, 0x6b, 0xfc, 0xb1, 0x92, 0xe8, 0xf2, 0xe8, 0x60, 
	0x38, 0x99, 0x53, 0xae, 0xc5, 0x31, 0x58, 0x5c, 0xaa, 0xee, 0x50, 0xbc, 0xd8, 0x51, 0x64, 0x06, 
	0x85, 0xdd, 0x16, 0xc4, 0xf4, 0x1a, 0xe6, 0xe3, 0x22, 0x8b, 0x06, 0x12, 0x37, 0x78, 0x91, 0xc0, 
	0x1f, 0x46, 0x03, 0x4a, 0x20, 0xba, 0xc5, 0x28, 0x58, 0xbe, 0x07, 0x26, 0x53, 0x2d, 0xf8, 0x96, 
	0xa8, 0xed, 0xf1, 0x8b, 0x21, 0xea, 0xdc, 0x80, 0xeb, 0xff, 0x99, 0xe5, 0x87, 0x07, 0x84, 0xc8, 
	0xc1, 0x45, 0x91, 0x57, 0xf6, 0xa7, 0xb7, 0x0d, 0x7f, 0x20, 0xb6, 0x00, 0x37, 0x09, 0x2f, 0xba, 
	0x72, 0x26, 0xc4, 0xab, 0xbe, 0x63, 0x27, 0x21, 0xb0, 0x25, 0x65, 0x16, 0x3d, 0xbc, 0xe5, 0x90, 
	0x40, 0x9c, 0x34, 0x93, 0x5a, 0x22, 0x4e, 0xfa, 0x21, 0x34, 0x88, 0xc6, 0xf7, 0xc0, 0x56, 0x1e, 
	0x5b, 0x54, 0x40, 0x21, 0xef, 0x29, 0x86, 0xf7, 0x45, 0xd3, 0xea, 0xba, 0x6d, 0xce, 0x99, 0x69, 
	0x8f, 0x82, 0x7b, 0x6d, 0x28, 0x4e, 0x5f, 0x5b, 0x5f, 0x14, 0xd8, 0x7c, 0x10, 0x92, 0xf7, 0xf1, 
	0x3c, 0xa4, 0x59, 0x23, 0x66, 0xc6, 0x32, 0x73, 0x4e, 0x6c, 0x60, 0x1f, 0x1b, 0x50, 0x76, 0x42, 
	0x5a, 0x05, 0x3b, 0xfd, 0x8a, 0x5a, 0x0e, 0x7f, 0xf1, 0x66, 0x05, 0xec, 0x4c, 0xdb, 0x39, 0xd5, 
	0xea, 0x12, 0xf4, 0x9f, 0x00, 0x4e, 0x5b, 0x67, 0x52, 0xaa, 0x09, 0xb1, 0x80, 0x4b, 0x69, 0x77, 
	0xe9, 0xc0, 0x51, 0x15, 0xfd, 0x7d, 0x77, 0xba, 0xbe, 0x63, 0xe2, 0x68, 0x7b, 0xed, 0x74, 0x3b, 
	0x6b, 0x34, 0x9e, 0x48, 0xe5, 0xce, 0x0d, 0xf9, 0x71, 0x4e, 0xe0, 0xbe, 0xcb, 0xac, 0x9a, 0xb1, 
	0x27, 0x8b, 0xf5, 0xd7, 0xa3, 0x14, 0xce, 0x31, 0x70, 0xb1, 0xdd, 0xfc, 0x61, 0x1c, 0xa6, 0xb6, 
	0xdb, 0xef, 0x98, 0xcf, 0x8d, 0x6a, 0x97, 0x05, 0x13, 0x44, 0x76, 0x79, 0xeb, 0xbe, 0xa6, 0x1c, 
	0x2b, 0x3d, 0x79, 0x9b, 0x05, 0xe6, 0x7c, 0x8f, 0xa9, 0xa1, 0xe4, 0x6a, 0xf7, 0x64, 0xb7, 0x48, 
	0x85, 0xe4, 0x7e, 0x28, 0xbd, 0x7e, 0x50, 0xf1, 0x93, 0xc2, 0x34, 0x5c, 0xd8, 0x23, 0xa5, 0x62, 
	0xc3, 0xe7, 0x9a, 0xa7, 0x2b, 0x3e, 0x82, 0xd0, 0x8a, 0x0a, 0xcd, 0xfe, 0x17, 0x03, 0x7c, 0xfb, 
	0xde, 0x8d, 0xfb, 0xef, 0x81, 0x62, 0x83, 0xa0, 0x18, 0xa5, 0x74, 0x3b, 0xbf, 0xfe, 0x21, 0x14, 
	0x0d, 0x67, 0x98, 0xd0, 0xe6, 0x88, 0x59, 0x0d, 0x46, 0xa1, 0x91, 0x2a, 0xf5, 0x0d, 0xae, 0x52, 
	0x78, 0x9f, 0xe4, 0x78, 0x36, 0xba, 0xae, 0x89, 0x90, 0x4a, 0x61, 0x9a, 0xd0, 0x66, 0x9a, 0x55, 
	0x96, 0xfa, 0x93, 0xf3, 0x54, 0x9e, 0x1f, 0xab, 0xb4, 0x0b, 0x6a, 0x96, 0x85, 0xb2, 0x3c, 0xbf, 
	0xc4, 0x19, 0x3d, 0x93, 0x29, 0xed, 0x15, 0x27, 0xc6, 0x02, 0xf5, 0xcc, 0xf9, 0xdc, 0xc6, 0x7c, 
	0xe5, 0xaa, 0x64, 0x25, 0x18, 0x4d, 0x6f, 0x30, 0x06, 0xa3, 0x6e, 0xcf, 0xa3, 0x85, 0xbb, 0x4a, 
	0xee, 0x1b, 0xe2, 0x55, 0x84, 0x11, 0x9f, 0x40, 0xa9, 0x4f, 0xeb, 0x5b, 0xb4, 0x78, 0x40, 0x3b, 
	0x00, 0x01, 0xbb, 0x5d, 0x70, 0x1f, 0xff, 0x84, 0x91, 0xdf, 0x1f, 0xcd, 0x67, 0xc1, 0xec, 0x6e, 
	0x7d, 0xe6, 0xce, 0x83, 0x45, 0x89, 0xc3, 0x7b, 0xb5, 0x9e, 0x77, 0x11, 0x2d, 0x36, 0x60, 0x60, 
	0x27, 0x4a, 0xe6, 0x17, 0x1e, 0x7f, 0x9e, 0xb5, 0xce, 0x7b, 0xda, 0x04, 0x8e, 0xda, 0x9f, 0x1d, 
	0x0a, 0x80, 0xe7, 0xfa, 0xc7, 0xae, 0x3f, 0x9e, 0x1f, 0x9b, 0x62, 0x9c, 0xc4, 0x83, 0x3e, 0x6f, 
	0x92, 0x51, 0x17, 0x0d, 0xee, 0xaa, 0x65, 0x53, 0x4a, 0x4b, 0x39, 0x8e, 0x87, 0x9c, 0x68, 0x19, 
	0x58, 0xc3, 0x67, 0x48, 0x4f, 0xe4, 0x18, 0x2a, 0x52, 0x8e, 0x74, 0xe2, 0x15, 0xf2, 0xba, 0x32, 
	0x3d, 0xe2, 0x42, 0x3e, 0xa6, 0x4c, 0x6c, 0x02, 0xfe, 0x35, 0xbb, 0x33, 0x0f, 0xee, 0xc9, 0xf8, 
	0x0d, 0x60, 0x2c, 0x65, 0x5f, 0x04, 0x93, 0x70, 0x13, 0x38, 0x85, 0xe0, 0x90, 0x74, 0x6c, 0x87, 
	0x98, 0xa8, 0xcd, 0xa9, 0x51, 0x56, 0x09, 0x9d, 0x7c, 0x93, 0xb7, 0xc9, 0x5a, 0x46, 0x36, 0x84, 
	0xb5, 0x62, 0x53, 0x99, 0x23, 0x80, 0x2d, 0x03, 0xce, 0x2b, 0x4d, 0x8f, 0x0b, 0x47, 0xce, 0xb5, 
	0x9b, 0xd0, 0x28, 0x09, 0xb4, 0x2d, 0x44, 0x3f, 0x77, 0x32, 0xba, 0x4e, 0x3d, 0xc7, 0x3c, 0x35, 
	0xda, 0xa2, 0xc4, 0xf5, 0x6d, 0x5a, 0xd4, 0x45, 0x8b, 0x60, 0x01, 0x59, 0x76, 0x8e, 0x8c, 0x84, 
	0x86, 0xf0, 0x8e, 0x08, 0x15, 0x17, 0x49, 0xf4, 0xb9, 0xcd, 0x75, 0x42, 0x91, 0x19, 0x7a, 0xfa, 
	0x0a, 0x3a, 0xb5, 0x51, 0xf6, 0x5f, 0x23, 0x97, 0xf2, 0x99, 0x0d, 0x74, 0xea, 0x71, 0x97, 0x74, 
	0xde, 0xe6, 0x7e, 0xfc, 0x5c, 0x53, 0xea, 0x91, 0x9b, 0xb5, 0xc1, 0x58, 0xec, 0x3a, 0x0a, 0x3a, 
	0xa4, 0xd0, 0x5c, 0x2c, 0x63, 0x4f, 0xda, 0xf7, 0xef, 0x4f, 0x67, 0xcd, 0xe1, 0xfc, 0x85, 0x77, 
	0xd6, 0xaa, 0xdf, 0xba, 0x3f, 0xa5, 0x60, 0xcb, 0x4f, 0x56, 0x26, 0x1c, 0xbc, 0x20, 0x16, 0x06, 
	0xa9, 0x86, 0x6b, 0x12, 0xbe, 0xfd, 0x9d, 0x0e, 0x4d, 0x5e, 0x06, 0xaf, 0x54, 0x15, 0x53, 0x81, 
	0x1a, 0xc9, 0xa7, 0xb4, 0xe5, 0x43, 0x0e, 0x1d, 0x57, 0x43, 0x08, 0xca, 0xb7, 0xd2, 0x98, 0x7d, 
	0x1d, 0x65, 0xf0, 0x70, 0xfd, 0xa9, 0x9d, 0x9c, 0x57, 0xe5, 0xdd, 0x4f, 0x3f, 0x27, 0x91, 0x92, 
	0x31, 0x1c, 0x0a, 0x45, 0xd9, 0x5f, 0x62, 0xb9, 0x5a, 0x0d, 0x93, 0x8d, 0x3a, 0x65, 0x2f, 0x1b, 
	0xac, 0xdd, 0x64, 0x9d, 0xe6, 0x13, 0xd8, 0x43, 0x67, 0xf9, 0x6a, 0x07, 0x90, 0x2f, 0x52, 0x3b, 
	0x4d, 0xa3, 0x04, 0xd3, 0x46, 0x23, 0x71, 0xc1, 0x3e, 0x1f, 0xbd, 0xaa, 0xc0, 0x92, 0xc9, 0x35, 
	0x63, 0x9b, 0x39, 0x13, 0x6b, 0xdf, 0x45, 0x44, 0xcb, 0x3a, 0x6f, 0xf1, 0x1b, 0xb6, 0x87, 0x0c, 
	0x9c, 0x00, 0xcc, 0xc5, 0x19, 0xcf, 0xcc, 0xc7, 0x08, 0x04, 0xc8, 0x8b, 0x3d, 0x87, 0x93, 0x8f, 
	0x91, 0x50, 0x08, 0x9c, 0x3f, 0xef, 0xb0, 0x57, 0x33, 0xf1, 0x03, 0x0c, 0x46, 0xb8, 0xa3, 0x1f, 
	0xbc, 0x5b, 0x4d, 0x9a, 0xbf, 0x50, 0xe8, 0x41, 0x90, 0xfb, 0x9d, 0x16, 0xd6, 0x68, 0x39, 0x25, 
	0xe9, 0x72, 0xb7, 0xe2, 0xd2, 0x35, 0xf5, 0x5c, 0xb5, 0x7c, 0xcb, 0x41, 0xbd, 0x0f, 0x61, 0x67, 
	0xcf, 0xb5, 0xa4, 0x59, 0xd0, 0x2b, 0x86, 0x8f, 0x09, 0xdc, 0x2e, 0xd2, 0x19, 0x08, 0xdd, 0x2e, 
	0x1f, 0xb5, 0x0a, 0x55, 0x51, 0x57, 0xa8, 0xe3, 0xf0, 0xaf, 0x70, 0xf3, 0x18, 0x54, 0x76, 0xa1, 
	0x06, 0xbc, 0x36, 0xad, 0xb6, 0x8d, 0xea, 0x58, 0xa3, 0xb3, 0x48, 0x14, 0xda, 0x0e, 0xd3, 0x0f, 
	0x29, 0x77, 0xd8, 0xbd, 0x72, 0xf9, 0x8f, 0x4e, 0x58, 0x8d, 0x83, 0x98, 0xcf, 0x0d, 0x27, 0xe6, 
	0xa7, 0x03, 0x20, 0x9a, 0x6f, 0xdf, 0xfe, 0xf8, 0xd2, 0xfb, 0x5e, 0x62, 0x95, 0xa5, 0x79, 0x6a, 
	0x80, 0xa1, 0xdf, 0xbc, 0x7e, 0xb7, 0x0e, 0x9a, 0xbd, 0xb2, 0x46, 0xe7, 0x44, 0x55, 0xc7, 0xf4, 
	0xae, 0x9e, 0x9b, 0x82, 0xa7, 0xe0, 0xa5, 0xfc, 0xbe, 0x16, 0x81, 0xb4, 0xf5, 0x17, 0xd2, 0x3a, 
	0x88, 0x13, 0x9b, 0x73, 0xd2, 0xa4, 0xa0, 0x68, 0x98, 0x1e, 0x02, 0xcd, 0x8a, 0x7a, 0xd9, 0x38, 
	0xa4, 0x38, 0x46, 0xe7, 0x00, 0xd0, 0x34, 0xf5, 0xdd, 0x2f, 0xd7, 0xdb, 0x7a, 0x16, 0xe5, 0x17, 
	0x9c, 0x0f, 0x1f, 0xc9, 0xf9, 0xca, 0xbb, 0x47, 0x9a, 0x1f, 0x04, 0x75, 0x70, 0xe0, 0x91, 0xda, 
	0xad, 0x50, 0xba, 0x39, 0x5a, 0x0c, 0x7a, 0xe1, 0x21, 0x53, 0xd5, 0xec, 0xe0, 0x38, 0xce, 0xc1, 
	0x31, 0xc6, 0x0a, 0x4c, 0xc0, 0x7d, 0x2b, 0x44, 0x92, 0xd6, 0x60, 0xc3, 0xa6, 0xe8, 0xb0, 0x8f, 
	0xeb, 0xe9, 0x0d, 0x0c, 0x66, 0xf6, 0x77, 0x03, 0xa3, 0x27, 0x27, 0xa4, 0x47, 0x1e, 0x28, 0xda, 
	0xe3, 0xa8, 0xc1, 0x8b, 0xf9, 0x07, 0x09, 0x49, 0x09, 0x04, 0x89, 0x58, 0x6f, 0x49, 0x4b, 0xa1, 
	0x4a, 0x79, 0xb0, 0x40, 0xa7, 0x62, 0x26, 0x03, 0x10, 0x76, 0x0f, 0xd7, 0x55, 0xf4, 0x02, 0x66, 
	0x06, 0x88, 0xd2, 0xb4, 0x61, 0x56, 0xf0, 0xb3, 0x90, 0xc9, 0xa1, 0x6f, 0x22, 0x6e, 0xb9, 0x6f, 
	0x16, 0x9b, 0x2e, 0x1f, 0xbd, 0x53, 0xa4, 0x5e, 0x99, 0x44, 0x7a, 0x15, 0x47, 0xbe, 0xa5, 0x4c, 
	0xc9, 0xf8, 0xc8, 0xf3, 0xa3, 0xda, 0x80, 0xa1, 0xd8, 0x5b, 0xba, 0xa4, 0x7d, 0x6c, 0xd8, 0x28, 
	0xaa, 0xed, 0xb0, 0xbb, 0x00, 0xd4, 0x97, 0x45, 0x61, 0xb3, 0x01, 0x56, 0xe8, 0x93, 0x61, 0xb9, 
	0x59, 0x69, 0x67, 0x3a, 0x2b, 0x06, 0xf3, 0x19, 0x56, 0xeb, 0xed, 0x5b, 0xf3, 0x9d, 0xaa, 0x04, 
	0xce, 0xa5, 0x75, 0x2f, 0xd3, 0x78, 0x6e, 0xd6, 0xb0, 0x83, 0x58, 0xdb, 0x61, 0xef, 0xbc, 0xb6, 
	0x56, 0x98, 0xd9, 0x0d, 0xc0, 0x6e, 0x57, 0x7d, 0xc0, 0x8f, 0x13, 0xdf, 0xb3, 0xa9, 0x1a, 0x0c, 
	0x45, 0x58, 0x62, 0x61, 0x39, 0xff, 0x31, 0x48, 0x98, 0xf6, 0x99, 0x28, 0x4b, 0x75, 0x8a, 0x02, 
	0x3e, 0xa3, 0x23, 0x9b, 0x38, 0xd1, 0xd6, 0xfc, 0x28, 0x10, 0xc3, 0x1a, 0x90, 0x93, 0x0d, 0xf2, 
	0x0e, 0xf4, 0x64, 0x36, 0x74, 0xc2, 0x47, 0xdf, 0x76, 0x7b, 0xe8, 0x81, 0x48, 0x6b, 0xb1, 0x7f, 
	0xd3, 0xb8, 0xdb, 0x3f, 0xd3, 0x61, 0xb6, 0x2b, 0x90, 0x17, 0x4a, 0x7b, 0x44, 0x88, 0xa9, 0xaf, 
	0x4e, 0x71, 0x00, 0xec, 0x8d, 0x00, 0xe3, 0x1c, 0x2a, 0x9a, 0xff, 0x7c, 0x4c, 0x04, 0xdd, 0xca, 
	0x06, 0xc4, 0x80, 0x0e, 0x28, 0x05, 0xb4, 0xac, 0x34, 0x69, 0xd3, 0xb5, 0x54, 0xda, 0x3b, 0x9d, 
	0xbb, 0x93, 0xc4, 0xd5, 0x2b, 0x7d, 0x95, 0xca, 0xab, 0x13, 0xad, 0x54, 0xc3, 0xf3, 0x5e, 0xe1, 
	0x3a, 0xd2, 0x00, 0x91, 0x78, 0xcc, 0xa5, 0xb5, 0x3d, 0x33, 0x44, 0x20, 0xd6, 0x20, 0x13, 0x01, 
	0xfc, 0x88, 0xdb, 0x48, 0x18, 0x2b, 0x29, 0x00, 0x7d, 0x41, 0x43, 0xc5, 0x6f, 0x8a, 0x04, 0x07, 
	0xbf, 0x72, 0x9f, 0xe6, 0xbc, 0xcd, 0xea, 0x64, 0x58, 0x84, 0x93, 0xc9, 0xee, 0x96, 0xed, 0x6f, 
	0x07, 0xde, 0xed, 0xab, 0xee, 0x76, 0xe2, 0x3d, 0x73, 0xc1, 0x58, 0xac, 0x75, 0xe7, 0xe6, 0x0a, 
	0x0b, 0xad, 0x64, 0xf3, 0xaa, 0x30, 0xb3, 0x0a, 0xa5, 0x3c, 0xc0, 0x5e, 0x56, 0x8f, 0x95, 0xcd, 
	0xfa, 0x3e, 0x6f, 0x27, 0x91, 0x60, 0xa7, 0x20, 0x93, 0x72, 0x25, 0xb9, 0x7a, 0xbb, 0xf4, 0xd8, 
	0xf6, 0x3e, 0xf5, 0xeb, 0x46, 0xb3, 0x97, 0x3a, 0xd2, 0x49, 0x84, 0xdc, 0xc3, 0x92, 0x32, 0x9d, 
	0xfe, 0x40, 0x3a, 0x8e, 0x90, 0xef, 0x18, 0x2e, 0x61, 0x94, 0x8e, 0xb1, 0x56, 0x1d, 0x90, 0x20, 
	0xc7, 0xc5, 0x35, 0xea, 0x69, 0x83, 0x85, 0x49, 0xc4, 0x9f, 0x60, 0x2b, 0x0a, 0x85, 0xed, 0xc6, 
	0x97, 0xab, 0xa3, 0xb9, 0x62, 0x00, 0xf7, 0xde, 0xbd, 0x3c, 0x91, 0x83, 0x26, 0x2a, 0xf8, 0x5b, 
	0xd0, 0x29, 0xdb, 0x1d, 0x88, 0x2b, 0xc4, 0x56, 0x70, 0x14, 0xb6, 0xd1, 0xc1, 0x7b, 0x5a, 0xf7, 
	0xb4, 0x74, 0x2a, 0x9b, 0x0b, 0xe3, 0x04, 0xd2, 0x50, 0x4e, 0x24, 0x4f, 0x13, 0x0d, 0x5e, 0x97, 
	0xeb, 0x66, 0xc5, 0xda, 0x5c, 0xfd, 0x8f, 0xf8, 0x16, 0x76, 0xef, 0xff, 0x8a, 0x5b, 0xd5, 0xcb, 
	0xf3, 0x35, 0xd6, 0x1e, 0x15, 0x4a, 0x04, 0xd1, 0x23, 0x8f, 0x0d, 0xef, 0x59, 0x51, 0x72, 0xaa, 
	0xf4, 0xa5, 0xdb, 0xaf, 0xbc, 0x4e, 0xf5, 0xca, 0xc8, 0x76, 0xea, 0x7a, 0x91, 0x7b, 0xbf, 0x65, 
	0x7b, 0x71, 0xb6, 0x6a, 0x72, 0x5c, 0xf6, 0x61, 0x51, 0xe3, 0x21, 0xf8, 0x87, 0xad, 0x98, 0x55, 
	0x15, 0xa2, 0xcb, 0xed, 0x86, 0x38, 0xa7, 0x63, 0x29, 0x5d, 0x68, 0xce, 0xa9, 0xae, 0x69, 0xba, 
	0x59, 0x99, 0xdf, 0xa1, 0x24, 0x81, 0xb5, 0x12, 0x31, 0xa7, 0x03, 0xa4, 0xfd, 0xd7, 0x8e, 0xe2, 
	0x8f, 0x15, 0x02, 0x9b, 0x53, 0x07, 0x39, 0x4a, 0x77, 0x52, 0x51, 0xe0, 0xd4, 0x81, 0xb0, 0x2d, 
	0x8c, 0x03, 0x14, 0x0b, 0xcd, 0x80, 0x86, 0x92, 0x52, 0x6b, 0x9f, 0x5a, 0xee, 0x2d, 0xb9, 0xe8, 
	0xf3, 0x2f, 0x9f, 0x26, 0x5c, 0x3b, 0x3c, 0xf9, 0x02, 0xf5, 0xc2, 0x68, 0x7b, 0x56, 0x1f, 0x20, 
	0x71, 0x6d, 0xc3, 0x34, 0x44, 0x49, 0xca, 0xfc, 0x85, 0xf6, 0xf1, 0xa2, 0x52, 0x71, 0x92, 0x4f, 
	0xaf, 0x76, 0x94, 0x2d, 0xc7, 0x83, 0xc4, 0x49, 0xc7, 0xaf, 0x01, 0xc6, 0x3c, 0x16, 0x86, 0xb3, 
	0x8b, 0x45, 0xe5, 0xa8, 0xa2, 0x45, 0xc7, 0xaf, 0x4c, 0xe4, 0x57, 0xc4, 0xe5, 0xc3, 0x30, 0x69, 
	0xf9, 0x4b, 0x7f, 0x6f, 0xe3, 0xe5, 0x6a, 0x5d, 0x59, 0x4f, 0x50, 0xe9, 0x38, 0xb1, 0x06, 0x29, 
	0xd2, 0x54, 0xb7, 0xb4, 0xfa, 0x16, 0x34, 0x79, 0x72, 0xcb, 0x52, 0x9c, 0x65, 0xeb, 0xce, 0x42, 
	0x7d, 0xb9, 0x12, 0xd2, 0xdd, 0x8c, 0x95, 0x98, 0x4f, 0x04, 0x89, 0xd9, 0x21, 0x97, 0x8b, 0xbd, 
	0x49, 0xd4, 0x95, 0x3b, 0xf3, 0x31, 0x12, 0x55, 0x2f, 0xf3, 0x4f, 0xf6, 0xf8, 0xb7, 0x3d, 0xc1, 
	0xca, 0xb1, 0x04, 0x07, 0x6c, 0xe4, 0x5a, 0x91, 0x1c, 0x5f, 0xa2, 0xbe, 0x2a, 0x89, 0x3b, 0xbc, 
	0x90, 0xb1, 0x54, 0x88, 0xde, 0x97, 0x2a, 0xc9, 0xcf, 0xa7, 0x6b, 0x2b, 0x50, 0x96, 0x2d, 0x8e, 
	0xf3, 0x5f, 0x39, 0xad, 0x00, 0x82, 0x45, 0x7a, 0x8b, 0x80, 0x4c, 0x0c, 0xdb, 0xb3, 0xda, 0xcb, 
	0xcb, 0xca, 0x6b, 0x26, 0xd1, 0xca, 0x53, 0xe0, 0x9e, 0x12, 0xef, 0xae, 0x4b, 0x5d, 0xa5, 0xc2, 
	0x18, 0xd4, 0x50, 0x7e, 0xba, 0x20, 0xec, 0x06, 0xb7, 0xaf, 0x57, 0x6d, 0xc1, 0x17, 0x60, 0x34, 
	0x22, 0xcd, 0x8c, 0x09, 0x64, 0x09, 0x1e, 0x89, 0x91, 0xee, 0xa0, 0xe4, 0x95, 0x5a, 0x16, 0x13, 
	0xdd, 0x71, 0x0a, 0x23, 0x79, 0x52, 0xd1, 0x94, 0x20, 0x37, 0x93, 0x53, 0xc3, 0xba, 0x6c, 0xd2, 
	0x7e, 0xa8, 0x87, 0x31, 0x04, 0x03, 0xfb, 0x4d, 0xa5, 0x81, 0xbe, 0x6d, 0xe1, 0xd5, 0x60, 0xd9, 
	0x73, 0x70, 0x9c, 0xd1, 0xe0, 0x4d, 0x16, 0x40, 0x0d, 0xd4, 0x4c, 0x9c, 0x00, 0x54, 0xf0, 0x85, 
	0xba, 0x8a, 0x07, 0x51, 0x73, 0xdc, 0xd9, 0x17, 0x2c, 0xe5, 0xce, 0x4e, 0xc4, 0xc2, 0x57, 0xcf, 
	0x18, 0xe1, 0x85, 0xf6, 0x2b, 0x33, 0x47, 0x3f, 0xf8, 0x63, 0x18, 0xad, 0x0c, 0x92, 0x6d, 0xaa, 
	0x5f, 0xe0, 0x17, 0xf1, 0xfa, 0x44, 0x66, 0xa2, 0xf6, 0xec, 0x69, 0x66, 0x40, 0x37, 0xbb, 0xdc, 
	0x6f, 0xe5, 0x82, 0x06, 0xd8, 0x4e, 0x12, 0xf4, 0x27, 0x15, 0xca, 0xb8, 0x61, 0xcd, 0x7c, 0x8f, 
	0xae, 0xd8, 0x7c, 0x02, 0xfd, 0xa5, 0x45, 0xfa, 0x80, 0xd7, 0x0d, 0xeb, 0x8d, 0xa0, 0x84, 0x11, 
	0xb6, 0xf2, 0x92, 0x1f, 0xc1, 0xd5, 0x01, 0xb2, 0xc4, 0x23, 0x39, 0x93, 0x8c, 0xc2, 0x06, 0x96, 
	0xbb, 0x75, 0x63, 0x35, 0x0c, 0x34, 0xc8, 0x2f, 0x68, 0xa8, 0x57, 0x62, 0x7f, 0x32, 0x6d, 0xdf, 
	0x3a, 0x89, 0xaf, 0x06, 0xb9, 0x83, 0xfc, 0xec, 0xa9, 0xf2, 0xb3, 0x50, 0x4b, 0xff, 0xf2, 0x3b, 
	0xd8, 0x33, 0x13, 0xf4, 0xec, 0xc1, 0x95, 0xae, 0x69, 0x48, 0xd4, 0x2e, 0xec, 0xb2, 0x0a, 0x31, 
	0x51, 0xa3, 0x4f, 0x84, 0x45, 0xc5, 0x48, 0x54, 0x2c, 0x5b, 0x59, 0x19, 0x14, 0x76, 0x97, 0xff, 
	0x14, 0x58, 0x61, 0xa3, 0x25, 0xc6, 0x6d, 0xe6, 0xeb, 0x28, 0xb7, 0x50, 0xdc, 0x0a, 0xa7, 0x63, 
	0x41, 0xa0, 0x8f, 0xea, 0x48, 0x1f, 0x8c, 0x0d, 0x80, 0xf4, 0xd3, 0x49, 0xac, 0x3b, 0x03, 0x1a, 
	0x82, 0xd1, 0xe7, 0xf4, 0xd6, 0xa6, 0x3f, 0x17, 0xe6, 0xee, 0x2b, 0xe5, 0x22, 0x5f, 0x01, 0x49, 
	0x47, 0x99, 0xa0, 0x4b, 0xdf, 0x46, 0x24, 0xac, 0xad, 0xc1, 0x9f, 0x36, 0x80, 0xe2, 0x59, 0xd7, 
	0x1f, 0xe8, 0xea, 0xfc, 0x6e, 0xf3, 0x25, 0x15, 0xde, 0x64, 0x03, 0xcb, 0xf3, 0x84, 0x59, 0x56, 
	0xd2, 0x3f, 0xae, 0x2d, 0xc0, 0x7c, 0x49, 0xe6, 0xe7, 0xe5, 0x80, 0x8c, 0xce, 0x9d, 0x80, 0x61, 
	0x5a, 0x85, 0xef, 0x73, 0x81, 0x13, 0x39, 0x17, 0x20, 0xbf, 0xc1, 0x49, 0x85, 0x95, 0x55, 0xd4, 
	0x34, 0x54, 0xe1, 0x3a, 0xbc, 0xb9, 0xd9, 0x85, 0xbf, 0x15, 0xb0, 0x08, 0x08, 0x1b, 0x04, 0x86, 
	0x68, 0x48, 0x8f, 0x40, 0x65, 0x02, 0x48, 0x8f, 0xba, 0x76, 0x53, 0xe6, 0x8a, 0x54, 0xcd, 0xfb, 
	0x50, 0x45, 0x27, 0x03, 0x3f, 0x44, 0x22, 0x22, 0x5a, 0xe8, 0xe0, 0xaa, 0x95, 0x77, 0x8e, 0x7f, 
	0xec, 0x88, 0xed, 0x53, 0xa0, 0x9b, 0x8d, 0x13, 0x92, 0xc8, 0x69, 0x68, 0x11, 0x06, 0xe7, 0xe0, 
	0x48, 0xb5, 0xf2, 0xfc, 0x9d, 0xc1, 0xc6, 0x15, 0xc4, 0xba, 0x0d, 0xf3, 0x7c, 0x8c, 0x4e, 0x93, 
	0xbe, 0x2b, 0xd0, 0xea, 0x88, 0xb1, 0x39, 0x20, 0xcb, 0x83, 0xdb, 0x83, 0xa2, 0xf1, 0xff, 0x34, 
	0x01, 0xe8, 0x75, 0x9f, 0xd3, 0x54, 0x0e, 0xe2, 0x11, 0x70, 0x87, 0x3b, 0xea, 0xa4, 0xd4, 0xa8, 
	0xab, 0xd5, 0xc7, 0x3f, 0x12, 0x65, 0x28, 0xfc, 0x94, 0x93, 0xd8, 0x22, 0xeb, 0x2d, 0xea, 0xe7, 
	0xfb, 0xd3, 0xed, 0x62, 0xb4, 0xe8, 0x05, 0x0e, 0xc8, 0x2a, 0x4b, 0x21, 0x36, 0x24, 0xec, 0xd9, 
	0xe7, 0x21, 0xda, 0x36, 0xeb, 0x85, 0x4b, 0xbf, 0xa6, 0x31, 0xcb, 0x96, 0x7c, 0x16, 0xe1, 0xf5, 
	0xf5, 0xce, 0x6e, 0x44, 0x08, 0xcf, 0x3a, 0x40, 0x54, 0x8f, 0xf2, 0xb1, 0xd2, 0x8f, 0x6d, 0xcc, 
	0x46, 0x11, 0x6e, 0x3d, 0x22, 0xa4, 0xe3, 0x1d, 0x04, 0x96, 0x17, 0xbf, 0xd2, 0x59, 0xff, 0x14, 
	0xc3, 0xae, 0x1d, 0xd1, 0x6f, 0x25, 0x08, 0xe1, 0xbc, 0x20, 0xed, 0x9f, 0xa2, 0x5d, 0x8c, 0x8a, 
	0x0e, 0x01, 0x4f, 0xde, 0xa4, 0x62, 0x99, 0xe2, 0xdc, 0x4a, 0x23, 0x9e, 0xe0, 0x3a, 0x3c, 0x85, 
	0x0f, 0x7e, 0x13, 0xbc, 0x24, 0xf2, 0x3b, 0x91, 0x79, 0xb3, 0x0b, 0x70, 0xb0, 0xda, 0x75, 0x8d, 
	0x64, 0x56, 0xa3, 0xad, 0x21, 0xed, 0x13, 0xa6, 0xb8, 0xd7, 0xa9, 0xe9, 0xe7, 0x8d, 0x37, 0xe6, 
	0x79, 0x82, 0x09, 0x40, 0xaf, 0xe7, 0x93, 0x0b, 0xf9, 0x69, 0xf8, 0x2f, 0x73, 0xb6, 0xa4, 0x2f, 
	0x7e, 0xd8, 0x00, 0x62, 0x09, 0x93, 0xe1, 0xa9, 0x3d, 0x9e, 0x8b, 0x9e, 0x60, 0x20, 0x3e, 0x3a, 
	0x54, 0x36, 0x8c, 0x6c, 0xe4, 0x3d, 0x42, 0x7d, 0x49, 0xdd, 0x11, 0x47, 0x90, 0x5b, 0x62, 0x63, 
	0xba, 0x30, 0x46, 0x8d, 0xda, 0xd2, 0x04, 0x61, 0x52, 0xa8, 0xaf, 0x0d, 0xc5, 0xcf, 0x53, 0x79, 
	0xe0, 0x67, 0x58, 0x04, 0xc5, 0x3e, 0x64, 0x2b, 0x43, 0x8e, 0xf6, 0x63, 0x5b, 0xa4, 0x5a, 0x66, 
	0xb7, 0x40, 0x2b, 0x1e, 0x16, 0x98, 0x32, 0xca, 0x0b, 0x1f, 0x98, 0x9c, 0xa0, 0xd1, 0x2c, 0xcb, 
	0x3d, 0xe9, 0x14, 0x37, 0x1e, 0x02, 0x70, 0xfa, 0x52, 0x38, 0x92, 0x6f, 0x09, 0x8f, 0x95, 0xa6, 
	0x9d, 0x87, 0x23, 0xb1, 0x5a, 0xcc, 0x44, 0x0a, 0x52, 0xe0, 0x77, 0x4d, 0x37, 0xa7, 0x51, 0x5f, 
	0xce, 0x18, 0x8c, 0x28, 0x8b, 0x9c, 0x37, 0x50, 0x43, 0x9e, 0xc2, 0x01, 0x04, 0x3f, 0x39, 0x47, 
	0x21, 0xe5, 0x57, 0xc9, 0xc2, 0x1c, 0xd6, 0x49, 0xa6, 0xf6, 0xb6, 0xf4, 0x8c, 0xd4, 0xd9, 0x09, 
	0xd5, 0x8a, 0xb4, 0x93, 0x5c, 0xc9, 0x74, 0x22, 0xb7, 0xe7, 0x52, 0x09, 0xbc, 0x41, 0x50, 0xad, 
	0x67, 0x0d, 0xa5, 0x11, 0xdc, 0x56, 0x04, 0xc7, 0xe3, 0x13, 0x9d, 0xdf, 0xd9, 0x41, 0x41, 0xbb, 
	0x76, 0xd7, 0xde, 0x8a, 0xd0, 0x24, 0xa2, 0xd0, 0xa1, 0x10, 0xa5, 0xf5, 0x8e, 0xe6, 0x90, 0x3d, 
	0x0c, 0xa1, 0xe4, 0xba, 0xa0, 0x1b, 0x39, 0xfa, 0xdf, 0x6e, 0x83, 0x11, 0x7b, 0xb8, 0x69, 0xc0, 
	0xd6, 0x77, 0x5b, 0xf0, 0x23, 0x70, 0x6a, 0x96, 0xc9, 0x00, 0x42, 0x8d, 0xff, 0xca, 0x39, 0x7b, 
	0x7a, 0x72, 0x20, 0xcb, 0x5b, 0x18, 0x70, 0xfc, 0x94, 0x01, 0x33, 0xda, 0xda, 0x9c, 0xb7, 0xa2, 
	0x18, 0x68, 0x5b, 0xec, 0xbd, 0x00, 0xf1, 0x0e, 0xbf, 0x9a, 0x04, 0x52, 0xed, 0x0b, 0xef, 0x2b, 
	0x25, 0xeb, 0xc5, 0xf7, 0xca, 0x91, 0x61, 0x65, 0x06, 0x33, 0x69, 0xfa, 0x3f, 0x18, 0xc1, 0xa0, 
	0x34, 0x6f, 0x0d, 0xbc, 0x67, 0x62, 0x3b, 0xc3, 0x9d, 0x9b, 0xe7, 0x1f, 0x04, 0x79, 0x22, 0xc6, 
	0xe5, 0x35, 0x39, 0x73, 0xac, 0x8f, 0xc9, 0x12, 0x84, 0x77, 0x6c, 0x3d, 0xeb, 0x13, 0x23, 0x3d, 
	0xab, 0x4f, 0xfa, 0x59, 0x9a, 0xcc, 0x8b, 0xfc, 0xc2, 0x88, 0xad, 0x5c, 0xd4, 0xfb, 0xe7, 0xb2, 
	0x0c, 0xa8, 0x46, 0x3e, 0x29, 0xd9, 0x58, 0xbd, 0x2e, 0x68, 0x58, 0xb0, 0x63, 0x29, 0x2e, 0xda, 
	0x21, 0x96, 0x4f, 0x3f, 0x9e, 0x78, 0x19, 0x9f, 0xef, 0x61, 0xf1, 0xb8, 0x99, 0x79, 0x12, 0xee, 
	0xba, 0x82, 0xf2, 0x12, 0xf3, 0x4c, 0xc5, 0x15, 0x5c, 0xd1, 0x7e, 0xcc, 0x55, 0x4f, 0xf1, 0x6a, 
	0x8b, 0x36, 0x28, 0xfa, 0x41, 0x5c, 0xd7, 0x32, 0x54, 0x8a, 0xf0, 0xd9, 0xeb, 0xac, 0x8c, 0x4d, 
	0x9a, 0xfd, 0xd6, 0xed, 0xbb, 0x3d, 0x85, 0xf1, 0x41, 0xb7, 0xfa, 0x9a, 0xcd, 0x1b, 0xec, 0xb2, 
	0x32, 0xd0, 0x5c, 0x69, 0xa3, 0xe2, 0x21, 0x45, 0x69, 0x98, 0x06, 0xc5, 0x29, 0x09, 0x88, 0xdc, 
	0x17, 0x74, 0xbf, 0xcb, 0xd0, 0x52, 0x63, 0x3a, 0x95, 0x64, 0xc3, 0x1a, 0x9f, 0x01, 0x0b, 0x27, 
	0x24, 0x05, 0x7e, 0xac, 0x87, 0x0f, 0xa8, 0xf6, 0xb1, 0x70, 0x12, 0x4b, 0xec, 0x85, 0xe3, 0x00, 
	0xac, 0xee, 0xb2, 0x4b, 0x8a, 0xe6, 0x4e, 0x68, 0x28, 0x2a, 0xa4, 0xad, 0xf6, 0x83, 0x4b, 0xc2, 
	0xdd, 0x6e, 0x56, 0x27, 0xa0, 0x7e, 0x1b, 0x64, 0xd1, 0x8e, 0x4c, 0x2e, 0xc8, 0x03, 0x00, 0x77, 
	0x2d, 0x41, 0x0b, 0x82, 0x9e, 0xab, 0x57, 0xbd, 0x07, 0xff, 0xa7, 0x2e, 0x07, 0xc2, 0x0f, 0xf8, 
	0x46, 0x18, 0x1e, 0x06, 0x82, 0x91, 0x8b, 0x8f, 0xf9, 0x82, 0xfa, 0x7d, 0x29, 0x3a, 0x56, 0x30, 
	0x46, 0xc3, 0x6b, 0x86, 0x1e, 0x03, 0xd1, 0xca, 0x99, 0x90, 0xda, 0xca, 0x1c, 0x76, 0xf1, 0x60, 
	0x33, 0xe2, 0xf9, 0x34, 0xbf, 0x45, 0xfc, 0x1d, 0x8a, 0x1d, 0x79, 0x8e, 0xff, 0x8f, 0xee, 0xdd, 
	0xa6, 0xde, 0x71, 0x94, 0xfa, 0x96, 0x1d, 0xe4, 0x32, 0x0b, 0x3e, 0x37, 0x87, 0x3b, 0x37, 0x5e, 
	0x3e, 0x24, 0x6a, 0x88, 0xf8, 0x62, 0x94, 0xd1, 0x2b, 0x31, 0xa3, 0xa2, 0xa3, 0xa0, 0x75, 0x79, 
	0xcc, 0x00, 0x74, 0x20, 0x18, 0x70, 0x0b, 0x0a, 0xfc, 0x63, 0x46, 0x06, 0x02, 0x4d, 0xb0, 0x94, 
	0xe8, 0x2e, 0x54, 0x27, 0x1f, 0x31, 0x5c, 0x4d, 0xde, 0x2d, 0x96, 0x57, 0x1a, 0x32, 0x1f, 0xfd, 
	0x00, 0x2c, 0xeb, 0xdf, 0x7a, 0xd4, 0xf1, 0xb3, 0x31, 0x53, 0x46, 0x79, 0x92, 0x5a, 0x0d, 0x11, 
	0x82, 0xf9, 0xda, 0x98, 0xcb, 0xa3, 0xed, 0x1c, 0x0c, 0xb4, 0x6f, 0x04, 0x13, 0xa0, 0xac, 0x4e, 
	0xd5, 0x02, 0xfe, 0x7b, 0x85, 0x33, 0xfb, 0x65, 0xb8, 0x4b, 0xc7, 0xd3, 0x85, 0xd1, 0x3a, 0x3b, 
	0x00, 0xe2, 0x0f, 0x5e, 0x15, 0xd3, 0xbc, 0x90, 0xf1, 0x37, 0x92, 0xe5, 0x97, 0xb0, 0xb8, 0xef, 
	0x4a, 0xfa, 0xa6, 0xb3, 0x8b, 0x3b, 0x5f, 0xbf, 0xe9, 0xa2, 0x1d, 0x3c, 0x93, 0xe9, 0x21, 0xaf, 
	0x0a, 0xa5, 0xa3, 0x64, 0x65, 0x6f, 0xef, 0x7a, 0xf6, 0x87, 0x4f, 0xcb, 0x8d, 0x47, 0x61, 0xbe, 
	0x83, 0x89, 0x2f, 0x77, 0x2b, 0xca, 0xd2, 0x64, 0x12, 0xc1, 0xdf, 0x99, 0x7b, 0x80, 0x79, 0x1b, 
	0x01, 0xec, 0x63, 0xe7, 0xc0, 0x64, 0x44, 0x1d, 0x68, 0x2e, 0x99, 0x0b, 0xbf, 0xa0, 0x75, 0xa3, 
	0x23, 0xca, 0x49, 0xb8, 0x49, 0x0d, 0x09, 0xd7, 0x0c, 0xf9, 0xfc, 0x01, 0xcb, 0xdc, 0xe0, 0x80, 
	0xf4, 0xc9, 0x4a, 0xa5, 0x9d, 0x10, 0xea, 0xc4, 0xf3, 0xff, 0x40, 0x27, 0x36, 0x17, 0x57, 0x17, 
	0x5c, 0x8f, 0x1a, 0x71, 0xfa, 0x26, 0x21, 0xa0, 0x28, 0x82, 0xff, 0x00, 0x52, 0x3e, 0xdc, 0xa4, 
	0xba, 0x2c, 0xda, 0xbc, 0xe6, 0xfa, 0x23, 0xe4, 0x68, 0x0a, 0xf9, 0xd1, 0x3c, 0xab, 0xcd, 0x3e, 
	0x33, 0x22, 0xfb, 0x70, 0x1a, 0xe6, 0xfe, 0x82, 0x33, 0xa2, 0xcb, 0xa2, 0xca, 0x4e, 0x1f, 0xad, 
	0x3c, 0x9a, 0x1e, 0x1d, 0xe3, 0x2a, 0x0c, 0x1f, 0x13, 0x4a, 0x23, 0x25, 0xa8, 0x9c, 0x93, 0xe6, 
	0x82, 0x5f, 0x4d, 0x13, 0x73, 0x69, 0xa9, 0xc5, 0xec, 0x2e, 0x85, 0x18, 0x46, 0x8a, 0x0a, 0x04, 
	0x57, 0x9f, 0xe2, 0x5e, 0xa3, 0x00, 0xfc, 0xe5, 0x36, 0x67, 0x24, 0x73, 0x3f, 0x8a, 0x55, 0xaa, 
	0x9e, 0x6d, 0xac, 0x04, 0xc0, 0x22, 0xbc, 0xd7, 0xeb, 0x88, 0x0b, 0x05, 0xd4, 0x19, 0xe9, 0x65, 
	0x1b, 0x00, 0xfb, 0x17, 0xf0, 0x7e, 0x9f, 0xbf, 0x64, 0x33, 0x84, 0x6b, 0xe3, 0x67, 0xe3, 0xc7, 
	0x78, 0x91, 0x81, 0x20, 0x36, 0xee, 0xfa, 0xda, 0x22, 0x8f, 0x7e, 0x04, 0xd4, 0x70, 0xdb, 0x74, 
	0x61, 0xa8, 0xe1, 0xb5, 0xbf, 0xaf, 0x64, 0xaf, 0xe3, 0x43, 0x96, 0x37, 0x7f, 0x98, 0x24, 0x6c, 
	0xae, 0x1a, 0x9c, 0x3b, 0xb4, 0xcc, 0x9e, 0x24, 0xe9, 0xa3, 0xc0, 0x0b, 0x35, 0x88, 0xf4, 0xc1, 
	0x3d, 0x08, 0xc9, 0x91, 0x48, 0x37, 0x8f, 0xc8, 0xc6, 0x84, 0x8f, 0x5e, 0x80, 0x8e, 0xe0, 0xa4, 
	0x12, 0xa7, 0xd8, 0x67, 0x00, 0xe8, 0x04, 0x01, 0x79, 0x77, 0xe1, 0xd6, 0xa4, 0x03, 0xfb, 0x26, 
	0x8a, 0x05, 0x3f, 0x9e, 0xf4, 0x6c, 0xe8, 0x81, 0x6f, 0xae, 0xe5, 0x6a, 0xeb, 0x75, 0x84, 0xeb, 
	0x1a, 0xc6, 0x84, 0x70, 0x55, 0xb9, 0x3c, 0xd4, 0x0f, 0x0c, 0xc8, 0x47, 0xf2, 0xfa, 0xf5, 0x48, 
	0x08, 0xbf, 0xeb, 0x9a, 0xe8, 0x8d, 0xb9, 0x6a, 0x8b, 0xb2, 0x8e, 0x48, 0x4c, 0xc3, 0x4f, 0x55, 
	0xb1, 0x2f, 0x93, 0x48, 0xa3, 0x89, 0x74, 0x6f, 0x93, 0x04, 0x05, 0x19, 0x39, 0xb0, 0x13, 0x9b, 
	0xbd, 0x80, 0x27, 0x74, 0x87, 0xe3, 0xbc, 0xf2, 0xcb, 0xbd, 0xff, 0x33, 0x37, 0xf6, 0x68, 0xc0, 
	0x3d, 0x27, 0xda, 0x81, 0x55, 0xfb, 0xe4, 0x13, 0xfd, 0x54, 0x0c, 0x6b, 0x62, 0x6e, 0x5e, 0xec, 
	0x29, 0xa5, 0x72, 0xaa, 0x24, 0xa1, 0xef, 0x09, 0xdf, 0x7f, 0xff, 0x66, 0x5b, 0x36, 0x07, 0x0b, 
	0x93, 0x99, 0x12, 0x20, 0x3c, 0xa2, 0x4b, 0xc4, 0x81, 0xf7, 0x49, 0x19, 0x01, 0x24, 0x6c, 0xbe, 
	0xc6, 0x64, 0x9e, 0x15, 0x0f, 0xe6, 0xe9, 0x4a, 0x6c, 0xb7, 0x76, 0xd9, 0x5b, 0xdd, 0x41, 0x74, 
	0xf0, 0x78, 0x0c, 0xcc, 0x9f, 0x51, 0x6e, 0x7d, 0x52, 0xe8, 0xfa, 0x4b, 0x21, 0xcc, 0xfa, 0x7a, 
	0x8b, 0x3e, 0xbc, 0x7b, 0xa3, 0x09, 0x88, 0xa7, 0xad, 0x2c, 0xd8, 0xec, 0x0c, 0x5e, 0x5d, 0xd1, 
	0x42, 0x60, 0x68, 0x45, 0x49, 0xa4, 0xe6, 0xa3, 0x8f, 0xc4, 0xe9, 
};

static bool decode_lua()
{
	// 将值进行运行时解密
	AVRC4 rc4;
	av_rc4_init(&rc4, (uint8_t*)"egret_soft", 10*8, 1);
	av_rc4_crypt(&rc4, (uint8_t*)lua_bincode, (const uint8_t*)lua_bincode, sizeof(lua_bincode), NULL, 1);
	return true;
}
bool do_decode_lua = decode_lua();


// 传入线程的结构
struct LuaCalc {
	MachineCode* mc;
	unsigned int code1;
	unsigned int code2;
};

// lua内核计算代码
static bool get_luacode_core(MachineCode* mc, unsigned int& code1, unsigned int& code2);

// 计算线程
static UINT WINAPI ThreadProc(LPVOID param)
{
	JUNK_CODE_TWO
	JUNK_CODE_ONE
	LuaCalc* calc = reinterpret_cast<LuaCalc*>(param);

	JUNK_CODE_THREE
	DWORD test_calc = (DWORD)(calc);
	JUNK_CODE_ONE

	// test_calc&0x01 属于 junk code !
	if (test_calc&0x01) 
	{
		JUNK_CODE_ONE
		JUNK_CODE_ONE
		JUNK_CODE_ONE
		calc = (LuaCalc*)(((char*)calc) - 1);
		JUNK_CODE_ONE
		JUNK_CODE_ONE
	
		MachineCode junk_mc = *calc->mc;

		JUNK_CODE_TWO
		JUNK_CODE_ONE
		junk_mc.buffer[5] = 0x08;
		junk_mc.buffer[15] = calc->code1 & 0xff;
		junk_mc.buffer[16] = calc->code2 & 0xee;
		JUNK_CODE_ONE
		JUNK_CODE_ONE
		JUNK_CODE_ONE
		get_luacode_core(&junk_mc, calc->code1, calc->code2);
		JUNK_CODE_ONE
	}
	else if (calc)
	{
		JUNK_CODE_FOUR
		get_luacode_core(calc->mc, calc->code1, calc->code2);
	}


	JUNK_CODE_FIVE
	_endthreadex(0);
	return 0;
}

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

// 调试函数
static int printMessage (lua_State *lua)
{
	unsigned int x = lua_tonumber(lua, 1);
	printf("- value = 0x%x. !\n", x);
	return 0;
}

// char进行异或运算
static int lua_c_do(lua_State *lua)
{
	unsigned int x = lua_tonumber(lua, 1);
	unsigned int y = lua_tonumber(lua, 2);
	unsigned int value = x^y;

	JUNK_CODE_THREE

	unsigned char idx = value & 0xFF;
	idx += 1;
	lua_pushnumber(lua, idx);

	JUNK_CODE_FOUR
	return 1;
}


// 得到lua注册码
// 算法为有一个256的crc表
// 将上一次crc的值分解成2部分，低8位为low_crc
// 将上一次crc的值右移8位，即之前的高24位，记为high_crc
// mc值（for循环）和low_crc的值进行一次异或
// 然后得到一个下标，然后将下标处的CRC和上一次的high_crc做一次异或
// 如此循环整个machinecode长度即可
bool get_luacode(MachineCode* mc, unsigned int& code1, unsigned int& code2)
{
	LuaCalc lc = {0};
	lc.mc = mc;
	// 用线程 + lua计算验证码
	unsigned int thread_id = -1;

	////////////////////////////////////////
	// junk code，一个无用的线程，蛊惑调试者
	HANDLE thread_handle_none = (HANDLE)_beginthreadex(
		NULL,
		0,
		ThreadProc,
		(void*)0,
		0,
		&thread_id
	);
	if (thread_id) {
		lc.code1 = 0x20090501;
		lc.code2 = 0x19991001;
		Sleep(20);
	}
	// junk code，一个无用的线程，蛊惑调试者
	////////////////////////////////////////
	
	HANDLE thread_handle = (HANDLE)_beginthreadex(
		NULL,
		0,
		ThreadProc,
		(void*)(&lc),
		0,
		&thread_id
	);
	Sleep(20);

	////////////////////////////////////////
	// junk code，一个无用的线程，蛊惑调试者
	if (test_under_debug()) {
		HANDLE thread_handle_none2 = (HANDLE)_beginthreadex(
			NULL,
			0,
			ThreadProc,
			(void*)(((char*)&lc)+1),
			0,
			&thread_id
		);
		code1 = lc.code1;
		code2 = lc.code2;

		JUNK_CODE_ONE
		JUNK_CODE_ONE
		JUNK_CODE_ONE

		Sleep(20);

		code2 ^= 0xFF00FF00;
		code1 |= 0x00330033;
		JUNK_CODE_ONE
	}
	// junk code，一个无用的线程，蛊惑调试者
	////////////////////////////////////////

	////////////////////////////////////////
	if (WAIT_OBJECT_0==::WaitForSingleObject(thread_handle, INFINITE)) {
		JUNK_CODE_THREE
		code1 = lc.code1;
		code2 = lc.code2;
		JUNK_CODE_THREE
		return true;
	}
	return false;
}

bool get_luacode_core(MachineCode* mc, unsigned int& code1, unsigned int& code2)
{
	// 注意所有test_under_debug保护的都是junk code
	// 并不会因为调试而影响实际的输出值
	JUNK_CODE_TWO
	JUNK_CODE_ONE

	// 用lua计算验证码
	int lua_error = 0;
	lua_State* L = lua_open();
	luaopen_bit(L);

#ifdef _DEBUG
	// 设置调试输出函数
	lua_pushcfunction(L, printMessage);
	lua_setglobal(L, "trace");
#endif

	JUNK_CODE_FIVE
	JUNK_CODE_ONE

	// 压入全局函数c_do
	if (!test_under_debug())
	{
		JUNK_CODE_ONE
		JUNK_CODE_ONE
	}

	// 压入全局函数c_do
	lua_pushcfunction(L, lua_c_do);
	JUNK_CODE_ONE
	lua_setglobal(L, "c_do");

	// 加载lua代码
	if (luaL_loadbuffer(L, (char*)lua_bincode, sizeof(lua_bincode), "egret")||lua_pcall(L, 0, 0, 0))
	//if (luaL_loadfile(L, "c:\\test.lua")||lua_pcall(L, 0, 0, 0))
	{
#ifdef _DEBUG
		fprintf(stderr, "%s", lua_tostring(L, -1));
#endif
	} 
	else 
	{
		code1 = 0xa86a7bc3;		// junk code
		code2 = 0xfe6a8bc3;		// junk code

		// 压入lua的do_calc函数
		lua_getglobal(L, "do_calc");

		JUNK_CODE_ONE
		JUNK_CODE_ONE

		// junk code
		if (!test_under_debug())
		{
			code1 = code1 ^ code2;
			code2 = (code2 >> 8) % 255 + code1;
		}

		// 新建一个数据表
		lua_newtable(L);		
		for (int i=0;i<_countof(mc->buffer);++i)
		{
			lua_pushnumber(L, i+1);
			lua_pushnumber(L, mc->buffer[i]);
			lua_settable(L, -3);

			// junk code
			JUNK_CODE_ONE
			code1 += i;
			code2 -= i;
		}

		JUNK_CODE_ONE
		// junk code
		if (!test_under_debug()) 
		{
			// 计算lf.key[2]中1的个数，保存在lf.key[3]最后一个字节中
			unsigned int x = code2;
			x = (x & 0x55555555UL) + ((x >> 1) & 0x55555555UL);
			x = (x & 0x33333333UL) + ((x >> 2) & 0x33333333UL);
			x = (x & 0x0f0f0f0fUL) + ((x >> 4) & 0x0f0f0f0fUL);
			x = (x & 0x00ff00ffUL) + ((x >> 8) & 0x00ff00ffUL);
			x = (x & 0x0000ffffUL) + ((x >> 16) & 0x0000ffffUL);
			code1 = x;
		}
		JUNK_CODE_ONE

		// 调用do_calc函数
		lua_error = lua_pcall(L, 1, 2, 0);
		if (lua_error)
		{
#ifdef _DEBUG
			fprintf(stderr, "%s", lua_tostring(L, -1));
#endif
		}
		else
		{
			JUNK_CODE_TWO
			code1 = lua_tonumber(L, -2);
			JUNK_CODE_ONE

			JUNK_CODE_FOUR
			JUNK_CODE_ONE
			code2 = 0xfe6a8bc3;		// junk code

			// 如果不在调试中，那么就弹出第二个值
			if (!test_under_debug())
			{
				JUNK_CODE_ONE
				code2 = lua_tonumber(L, -1);
				JUNK_CODE_FOUR
			}

			JUNK_CODE_ONE
			JUNK_CODE_TWO
			code2 = lua_tonumber(L, -1);
		}
	}

	// 关闭
	lua_settop(L, 0);
	lua_close(L);
	
	return true;
}


// 同样的算法，用C实现
bool get_luacode_c(MachineCode* mc, unsigned int& code1, unsigned int& code2)
{
	// CRC表
	const unsigned int AV_CRC_32_IEEE[] = 
	{
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
		0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D, 0x00000001
    };

	int i;
	unsigned int max_value = 0;

	code1 = 0;
	code2 = 0;

	for (i=0;i<sizeof(mc->buffer);++i)
	{
		unsigned char idx = code1 ^ mc->buffer[i];
		
		if (AV_CRC_32_IEEE[idx]>max_value) {
			max_value = AV_CRC_32_IEEE[idx];
		}

		int tt = code1 >> 8;
		code1 = AV_CRC_32_IEEE[idx] ^ tt;
	}

    if (sizeof(mc->buffer)>3)
	{
		int part0 = max_value >> 24;
		int part1 = part0 & mc->buffer[0] | mc->buffer[1];
    	int part2 = part1 & mc->buffer[2] | mc->buffer[3];
    	int part3 = part2 & mc->buffer[4] | mc->buffer[5];
    	code2 = part1 + part2*256 + part3*65536;
	}

	return 0;
}



#if 0
// 原始lua源代码

function do_calc(data)
	local AV_CRC_32_IEEE_LE = 
	{	
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
		0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D, 0x00000001
    }
    
	value1 = 0
	value2 = 0
	local max_value = 0
		
    for i=1, #data do
		local idx = c_do(value1, data[i])
		if AV_CRC_32_IEEE_LE[idx]>max_value then
			max_value = AV_CRC_32_IEEE_LE[idx]
		end
		value1 = bit.bxor(AV_CRC_32_IEEE_LE[idx], bit.rshift(value1, 8))
    end
    
    if #data>3 then
    	local part0 = bit.rshift(max_value, 24)
    	local part1 = bit.bor(bit.band(part0, data[1]), data[2])
    	local part2 = bit.bor(bit.band(part1, data[3]), data[4])    	
    	local part3 = bit.bor(bit.band(part2, data[5]), data[6])
    	value2 = part1 + part2*256 + part3*65536
    end
    
	return value1, value2
end


#endif