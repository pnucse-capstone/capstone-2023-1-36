import os
import pickle
import copy
import struct

def u16( x ):
    return struct.unpack( '<H', x )[0]

mtp_file_path = "./target_mtp.MTP"

file_size = os.path.getsize( mtp_file_path )
print( "file_size :", file_size, hex( file_size ) )

with open( mtp_file_path, "rb" ) as f:
    mtp_data = f.read()

print( "size :", len( mtp_data ) )

mtp_index = 0
mtp_index_tmp = 0
cur_mtp_index = 0
parsed_data = []

while mtp_index < len( mtp_data ):
    cur_mtp_index = mtp_index_tmp + 3
    mtp_index_tmp = cur_mtp_index
    size_1 = u16( mtp_data[mtp_index + 1:mtp_index + 3] )
    size_2 = size_1 + 3
    size_3 = size_1 + 3
    size_4 = size_1 + cur_mtp_index
    if size_1 + cur_mtp_index <= len( mtp_data ):
        size_5 = size_3
        new_arr = []
        new_arr.append( mtp_data[mtp_index+0] )
        new_arr.append( mtp_data[mtp_index+1] )
        new_arr.append( mtp_data[mtp_index+2] )
        for i in range( size_1 ):
            new_arr.append( mtp_data[mtp_index+3+i] )
        mtp_index += size_5
        size_2 = size_3
        mtp_index_tmp = size_4

    parsed_data.append( copy.deepcopy( new_arr ) )

for d in parsed_data:
    first_byte = d[0]
    size = d[1] + d[2] * 16 ** 2
    data = d[3:]
    data_size = len( data )
    section_size = len( d )
    print( "first byte :", hex( d[0] ) )
    print( "data size( size ) :", data_size, hex( data_size ), "(", size, ")" )
    print( "section size :", section_size, hex( section_size ) )

    tmp1 = first_byte - 0x65
    if tmp1 != 0:
        tmp2 = tmp1 - 0x23
        if tmp2 != 0:
            if tmp2 == 0x38:
                tmp_section_arr = copy.deepcopy( d )
                # setting mcu info ( just alloc, not data is setted
                size_tmp1 = 0x8000
                size_tmp2 = 0x40
                # program_in_data_2 size == 0x8000 ( size_tmp1 )
        else:
            if d[4] == 2:
                int16_data = d[5] + d[6] * 16 ** 2
            if d[4] == 0x10:
                pass
            if d[4] == 0x12:
                if d[5] == ord('e'):
                    int8_data = 1
                    nBootLoaderSize = 512
                if d[5] == ord('f'):
                    int8_data = 2
                    nBootLoaderSize = 512
                if d[5] == ord('q'):
                    int8_data = 1
                    nBootLoaderSize = 1024
                if d[5] == ord('r'):
                    int8_data = 2
                    nBootLoaderSize = 1024





