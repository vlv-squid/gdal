/******************************************************************************
 *
 * Project:  GeoTIFF Driver
 * Purpose:  GDAL GeoTIFF support.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1998, 2002, Frank Warmerdam <warmerdam@pobox.com>
 * Copyright (c) 2010-2013, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#ifndef GTIFF_H_INCLUDED
#define GTIFF_H_INCLUDED

#include "cpl_port.h"
#include "cpl_string.h"
#include <cstdint>

#include "gdal.h"
#include "tiffio.h"
#include "tiffvers.h"

CPL_C_START
void CPL_DLL GTiffOneTimeInit();
void CPL_DLL LibgeotiffOneTimeInit();
CPL_C_END

void GTIFFSetThreadLocalInExternalOvr(bool b);
void GTIFFGetOverviewBlockSize(GDALRasterBandH hBand, int *pnBlockXSize,
                               int *pnBlockYSize);
void GTIFFSetJpegQuality(GDALDatasetH hGTIFFDS, int nJpegQuality);
void GTIFFSetWebPLevel(GDALDatasetH hGTIFFDS, int nWebPLevel);
void GTIFFSetJpegTablesMode(GDALDatasetH hGTIFFDS, int nJpegTablesMode);
void GTIFFSetWebPLossless(GDALDatasetH hGTIFFDS, bool bWebpLossless);
void GTIFFSetZLevel(GDALDatasetH hGTIFFDS, int nZLevel);
void GTIFFSetZSTDLevel(GDALDatasetH hGTIFFDS, int nZSTDLevel);
void GTIFFSetMaxZError(GDALDatasetH hGTIFFDS, double dfMaxZError);
#if HAVE_JXL
void GTIFFSetJXLLossless(GDALDatasetH hGTIFFDS, bool bIsLossless);
void GTIFFSetJXLEffort(GDALDatasetH hGTIFFDS, int nEffort);
void GTIFFSetJXLDistance(GDALDatasetH hGTIFFDS, float fDistance);
void GTIFFSetJXLAlphaDistance(GDALDatasetH hGTIFFDS, float fAlphaDistance);
#endif
const char *GTIFFGetCompressionMethodName(int nCompressionCode);
int GTIFFGetCompressionMethod(const char *pszValue,
                              const char *pszVariableName);
bool GTIFFSupportsPredictor(int nCompression);
bool GTIFFUpdatePhotometric(const char *pszPhotometric,
                            const char *pszOptionKey, int nCompression,
                            const char *pszInterleave, int nBands,
                            uint16_t &nPhotometric, uint16_t &nPlanarConfig);

void GTiffDatasetWriteRPCTag(TIFF *hTIFF, char **papszRPCMD);
char **GTiffDatasetReadRPCTag(TIFF *hTIFF);

void GTiffWriteJPEGTables(TIFF *hTIFF, const char *pszPhotometric,
                          const char *pszJPEGQuality,
                          const char *pszJPEGTablesMode);
CPLString GTiffFormatGDALNoDataTagValue(double dfNoData);

// Note: Was EXTRASAMPLE_ASSOCALPHA in GDAL < 1.10.
constexpr uint16_t DEFAULT_ALPHA_TYPE = EXTRASAMPLE_UNASSALPHA;

uint16_t GTiffGetAlphaValue(const char *pszValue, uint16_t nDefault);

bool GTIFFIsStandardColorInterpretation(GDALDatasetH hSrcDS,
                                        uint16_t nPhotometric,
                                        CSLConstList papszCreationOptions);

CPLString CPL_DLL GTiffGetCompressValues(bool &bHasLZW, bool &bHasDEFLATE,
                                         bool &bHasLZMA, bool &bHasZSTD,
                                         bool &bHasJPEG, bool &bHasWebP,
                                         bool &bHasLERC, bool bForCOG);

int &GTIFFGetThreadLocalLibtiffError();

#if !defined(TIFFTAG_GDAL_METADATA)
// The following 5 tags are now defined in tiff.h of libtiff > 4.1.0

#define TIFFTAG_GDAL_METADATA 42112
#define TIFFTAG_GDAL_NODATA 42113
#define TIFFTAG_RPCCOEFFICIENT 50844

/* GeoTIFF DGIWG */
/* https://www.awaresystems.be/imaging/tiff/tifftags/tiff_rsid.html */
#define TIFFTAG_TIFF_RSID 50908
/* https://www.awaresystems.be/imaging/tiff/tifftags/geo_metadata.html */
#define TIFFTAG_GEO_METADATA 50909

#endif

#if !defined(PREDICTOR_NONE)
#define PREDICTOR_NONE 1
#endif

#if !defined(COMPRESSION_LZMA)
#define COMPRESSION_LZMA 34925 /* LZMA2 */
#endif

#if !defined(TIFFTAG_LZMAPRESET)
#define TIFFTAG_LZMAPRESET 65562 /* LZMA2 preset (compression level) */
#endif

#if !defined(COMPRESSION_ZSTD)
#define COMPRESSION_ZSTD 50000 /* ZSTD */
#endif

#if !defined(TIFFTAG_ZSTD_LEVEL)
#define TIFFTAG_ZSTD_LEVEL 65564 /* ZSTD compression level */
#endif

#if !defined(COMPRESSION_LERC)
#define COMPRESSION_LERC 34887 /* LERC */
#endif

#ifndef TIFFTAG_LERC_VERSION
#define TIFFTAG_LERC_PARAMETERS                                                \
    50674 /* Stores LERC version and additional compression method */
#endif

#ifndef TIFFTAG_LERC_VERSION
/* Pseudo tags */
#define TIFFTAG_LERC_VERSION 65565 /* LERC version */
#define LERC_VERSION_2_4 4
#define TIFFTAG_LERC_ADD_COMPRESSION 65566 /* LERC additional compression */
#define LERC_ADD_COMPRESSION_NONE 0
#define LERC_ADD_COMPRESSION_DEFLATE 1
#define LERC_ADD_COMPRESSION_ZSTD 2
#define TIFFTAG_LERC_MAXZERROR 65567 /* LERC maximum error */
#endif

#if !defined(COMPRESSION_WEBP)
#define COMPRESSION_WEBP 50001 /* WebP */
#endif

#if !defined(TIFFTAG_WEBP_LEVEL)
#define TIFFTAG_WEBP_LEVEL 65568 /* WebP compression level */
#endif

#if !defined(TIFFTAG_WEBP_LOSSLESS)
#define TIFFTAG_WEBP_LOSSLESS 65569 /* WebP lossless/lossy */
#endif

#if (TIFFLIB_VERSION > 20220520) || defined(INTERNAL_LIBTIFF)  // > 4.4.0
#define SUPPORTS_LIBTIFF_OPEN_OPTIONS
#endif

constexpr double DEFAULT_NODATA_VALUE = -9999.0;

#endif  // GTIFF_H_INCLUDED
