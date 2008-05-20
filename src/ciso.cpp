/*
	Compressed ISO9660 conveter by BOOSTER
*/

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <sstream>

#include "ciso.h"

using namespace std;


/****************************************************************************
	compress ISO to CSO
****************************************************************************/

CIso::CIso()
: index_buf(NULL)
, crc_buf(NULL)
, block_buf1(NULL)
, block_buf2(NULL)
, ciso_total_block(0)
, m_Progress(0)
, m_CompressionRate(0)
{
    
}

CIso::~CIso()
{
    // free memory
	if(index_buf) free(index_buf);
	if(crc_buf) free(crc_buf);
	if(block_buf1) free(block_buf1);
	if(block_buf2) free(block_buf2);
}

long CIso::check_file_size(FILE *fp, CISO_H* ciso)
{
	long pos;

	if(fseek(fp,0,SEEK_END) < 0)
	{
		return -1;
	}
	pos = ftell(fp);
	if(pos==-1)
	{
	    return pos;
	}

	// init ciso header
	memset(ciso,0,sizeof(*ciso));

	ciso->magic[0] = 'C';
	ciso->magic[1] = 'I';
	ciso->magic[2] = 'S';
	ciso->magic[3] = 'O';
	ciso->ver      = 0x01;

	ciso->block_size  = 0x800; /* ISO9660 one of sector */
	ciso->total_bytes = pos;

	ciso_total_block = pos / ciso->block_size ;
	
	fseek(fp,0,SEEK_SET);

	return pos;
}



/***************************************************************************
	decompress CSO to ISO
****************************************************************************/
void CIso::decompress(const std::string& filenameIn, const std::string& filenameOut)
{
	//unsigned long long file_size;
	unsigned int index, index2;
	unsigned long long read_pos, read_size;
	//int total_sectors;
	unsigned int index_size;
	int block;
	//unsigned char buf4[4];
	unsigned int cmp_size;
	int status;
	int percent_period;
	int percent_cnt;
	int plain;
	CISO_H ciso;
	z_stream z;
	FILE *fin,*fout;

	m_Progress = 0;
	m_CompressionRate = 0;
	
	if((fin = fopen(filenameIn.c_str(), "rb")) == NULL)
	{
		throw CIsoException("Can't open " + filenameIn);
	}
	if((fout = fopen(filenameOut.c_str(), "wb")) == NULL)
	{
	    throw CIsoException("Can't create " + filenameOut);
	}

	// read header
	if( fread(&ciso, 1, sizeof(ciso), fin) != sizeof(ciso) )
	{
	    throw CIsoException("File read error");
	}

	// check header
	if(
		ciso.magic[0] != 'C' ||
		ciso.magic[1] != 'I' ||
		ciso.magic[2] != 'S' ||
		ciso.magic[3] != 'O' ||
		ciso.block_size ==0  ||
		ciso.total_bytes == 0
	)
	{
	    throw CIsoException("ciso file format error");
	}

	ciso_total_block = ciso.total_bytes / ciso.block_size;

	// allocate index block
	index_size = (ciso_total_block + 1 ) * sizeof(unsigned long);
	index_buf  = (unsigned int*) malloc(index_size);
	block_buf1 = (unsigned char*) malloc(ciso.block_size);
	block_buf2 = (unsigned char*) malloc(ciso.block_size*2);

	if( !index_buf || !block_buf1 || !block_buf2 )
	{
	    throw CIsoException("Can't allocate memory");
	}
	memset(index_buf,0,index_size);

	// read index block
	if( fread(index_buf, 1, index_size, fin) != index_size )
	{
	    throw CIsoException("file read error");
	}

	// init zlib
	z.zalloc = Z_NULL;
	z.zfree  = Z_NULL;
	z.opaque = Z_NULL;

	// decompress data
	percent_period = ciso_total_block/100;
	percent_cnt = 0;


	for(block = 0;block < ciso_total_block ; block++)
	{
		if(--percent_cnt<=0)
		{
			percent_cnt = percent_period;
			m_Progress = block / percent_period;
		}

		if(inflateInit2(&z,-15) != Z_OK)
		{
		    throw CIsoException(string("inflateInit : ") + ((z.msg) ? string(z.msg) : string("???")));
		}

		// check index
		index  = index_buf[block];
		plain  = index & 0x80000000;
		index  &= 0x7fffffff;
		read_pos = index << (ciso.align);
		if(plain)
		{
			read_size = ciso.block_size;
		}
		else
		{
			index2 = index_buf[block+1] & 0x7fffffff;
			read_size = (index2-index) << (ciso.align);
		}
		fseek(fin,read_pos,SEEK_SET);

		z.avail_in  = fread(block_buf2, 1, read_size , fin);
		if(z.avail_in != read_size)
		{
		    stringstream ss;
		    ss << "block=" << block << " : read error";
		    throw CIsoException(ss.str());
		}

		if(plain)
		{
			memcpy(block_buf1,block_buf2,read_size);
			cmp_size = read_size;
		}
		else
		{
			z.next_out  = block_buf1;
			z.avail_out = ciso.block_size;
			z.next_in   = block_buf2;
			status = inflate(&z, Z_FULL_FLUSH);
			if (status != Z_STREAM_END)
			{
			    stringstream ss;
			    ss << "block " << block << ":inflate : " << ((z.msg) ? z.msg : "error") << "[" << status << "]";
			    throw CIsoException(ss.str());
			}
			cmp_size = ciso.block_size - z.avail_out;
			if(cmp_size != ciso.block_size)
			{
				stringstream ss;
			    ss << "block " << block << ": block size error " << cmp_size << " != " << ciso.block_size;
			    throw CIsoException(ss.str());
			}
		}
		// write decompressed block
		if(fwrite(block_buf1, 1,cmp_size , fout) != cmp_size)
		{
		    stringstream ss;
		    ss << "block " << block << " : Write error"; 
		    throw CIsoException(ss.str());
		}

		// term zlib
		if (inflateEnd(&z) != Z_OK)
		{
	    	    throw CIsoException(string("inflateEnd : ") + ((z.msg) ? string(z.msg) : string("error")));
		}
	}

	m_Progress = 100;

	// close files
	fclose(fin);
	fclose(fout);
}

/****************************************************************************
	compress ISO
****************************************************************************/
void CIso::compress(const std::string& filenameIn, const std::string& filenameOut, int level)
{
	long file_size;
	long write_pos;
	//int total_sectors;
	int index_size;
	int block;
	unsigned char buf4[64];
	unsigned int cmp_size;
	int status;
	int percent_period;
	int percent_cnt;
	unsigned int align,align_b,align_m;
	CISO_H ciso;
	z_stream z;
	FILE *fin,*fout;

	m_Progress = 0;
	m_CompressionRate = 0;
	
	if((fin = fopen(filenameIn.c_str(), "rb")) == NULL)
	{
		throw CIsoException("Can't open " + filenameIn);
	}
	if((fout = fopen(filenameOut.c_str(), "wb")) == NULL)
	{
	    throw CIsoException("Can't create " + filenameOut);
	}

	file_size = check_file_size(fin, &ciso);
	if(file_size < 0)
	{
		throw CIsoException("Can't get file size");
	}

	// allocate index block
	index_size = (ciso_total_block + 1 ) * sizeof(unsigned long);
	index_buf  = (unsigned int*) malloc(index_size);
	crc_buf    = (unsigned int*) malloc(index_size);
	block_buf1 = (unsigned char*) malloc(ciso.block_size);
	block_buf2 = (unsigned char*) malloc(ciso.block_size*2);

	if(!index_buf || !crc_buf || !block_buf1 || !block_buf2)
	{
		throw CIsoException("Can't allocate memory");
	}
	memset(index_buf,0,index_size);
	memset(crc_buf,0,index_size);
	memset(buf4,0,sizeof(buf4));

	// init zlib
	z.zalloc = Z_NULL;
	z.zfree  = Z_NULL;
	z.opaque = Z_NULL;

	// write header block
	fwrite(&ciso,1,sizeof(ciso),fout);

	// dummy write index block
	fwrite(index_buf,1,index_size,fout);

	write_pos = sizeof(ciso) + index_size;

	// compress data
	percent_period = ciso_total_block/100;
	percent_cnt    = ciso_total_block/100;

	align_b = 1<<(ciso.align);
	align_m = align_b -1;

	for(block = 0;block < ciso_total_block ; block++)
	{
		if(--percent_cnt<=0)
		{
			percent_cnt = percent_period;
			m_Progress = block / percent_period;
			m_CompressionRate = block == 0 ? 0 : 100 - (int)(100.f * write_pos / (block * 2048));
			if(m_CompressionRate < 0)
			{
				m_CompressionRate = 0;
			}
		}

		if(deflateInit2(&z, level , Z_DEFLATED, -15,8,Z_DEFAULT_STRATEGY) != Z_OK)
		{
			throw CIsoException(string("deflateInit : ") + ((z.msg) ? string(z.msg) : string("???")));
		}

		// write align
		align = (int)write_pos & align_m;
		if(align)
		{
			align = align_b - align;
			if(fwrite(buf4,1,align, fout) != align)
			{
				stringstream ss;
		        ss << "block=" << block << " : write error";
		        throw CIsoException(ss.str());
			}
			write_pos += align;
		}

		// mark offset index
		index_buf[block] = write_pos>>(ciso.align);

		// read buffer
		z.next_out  = block_buf2;
		z.avail_out = ciso.block_size*2;
		z.next_in   = block_buf1;
		z.avail_in  = fread(block_buf1, 1, ciso.block_size , fin);
		if(z.avail_in != ciso.block_size)
		{
			stringstream ss;
		    ss << "block=" << block << " : read error";
		    throw CIsoException(ss.str());
		}

		// compress block
		status = deflate(&z, Z_FINISH);
		if (status != Z_STREAM_END)
		{
		    stringstream ss;
			ss << "block " << block << ":deflate : " << ((z.msg) ? z.msg : "error") << "[" << status << "]";
			throw CIsoException(ss.str());
		}

		cmp_size = ciso.block_size*2 - z.avail_out;

		// choise plain / compress
		if(cmp_size >= ciso.block_size)
		{
			cmp_size = ciso.block_size;
			memcpy(block_buf2,block_buf1,cmp_size);
			// plain block mark
			index_buf[block] |= 0x80000000;
		}

		// write compressed block
		if(fwrite(block_buf2, 1,cmp_size , fout) != cmp_size)
		{
			stringstream ss;
		    ss << "block=" << block << " : write error";
		    throw CIsoException(ss.str());
		}

		// mark next index
		write_pos += cmp_size;

		// term zlib
		if (deflateEnd(&z) != Z_OK)
		{
			throw CIsoException(string("deflateEnd : ") + ((z.msg) ? z.msg : "error"));
		}
	}

	// last position (total size)
	index_buf[block] = write_pos>>(ciso.align);

	// write header & index block
	fseek(fout,sizeof(ciso),SEEK_SET);
	fwrite(index_buf,1,index_size,fout);

	m_Progress = 100;

    // close files
	fclose(fin);
	fclose(fout);
}
