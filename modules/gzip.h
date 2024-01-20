#ifndef GZIP_H
#define GZIP_H
#ifdef __EMSCRIPTEN__
#include"/usr/include/zlib.h"
#else 
#include<zlib.h>
#endif

using std::string;
using std::stringstream;

#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

buffer compress_gzip(buffer str, int compressionlevel = Z_BEST_COMPRESSION) {
    // cout << str.asString() << endl;
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit2(&zs, 
                     compressionlevel,
                     Z_DEFLATED,
                     MOD_GZIP_ZLIB_WINDOWSIZE + 16, 
                     MOD_GZIP_ZLIB_CFACTOR,
                     Z_DEFAULT_STRATEGY) != Z_OK
    ) throw(std::runtime_error("deflateInit2 failed while compressing."));
    char* x = new char[str.size()];
    for (int i = 0; i < str.size(); i++) x[i] = str.v[i];
    zs.next_in = (Bytef*)x;
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out) outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    } delete[] x;
    return buffer(outstring);
}

buffer compress_deflate(buffer str, int compressionlevel = Z_BEST_COMPRESSION) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
	return buffer(outstring);
}

buffer decompress_deflate(buffer str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    return buffer(outstring);
}

buffer decompress_gzip(buffer str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE + 16) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));
    char* x = new char[str.size()];
    for (int i = 0; i < str.size(); i++) x[i] = str.v[i];
    zs.next_in = (Bytef*)x;
    zs.avail_in = str.size();
    int ret;
    char outbuffer[100010];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    } delete[] x;
    return buffer(outstring);
}

#endif
