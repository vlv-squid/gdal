/******************************************************************************
 *
 * Project:  GDAL Core
 * Purpose:  Implementation of GDALAllRegister(), primary format registration.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1998, Frank Warmerdam
 * Copyright (c) 2007-2014, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#include "gdal_priv.h"
#include "gdal_frmts.h"
#include "ogrsf_frmts.h"

#ifdef GNM_ENABLED
#include "gnm_frmts.h"
#endif

#if defined(HAVE_EXTERNAL_DEFERRED_PLUGINS)
// The above define and the below functions are set and generated by
// frmts/CMakeLists.txt when a CMake ADD_EXTERNAL_DEFERRED_PLUGIN_XXXX
// variable is set.
extern "C" void DeclareExternalDeferredPlugins(void);
#endif

extern "C" void CPL_DLL GDALRegister_raw_no_sidecar();
extern "C" void CPL_DLL GDALRegister_raw_with_sidecar();

/************************************************************************/
/*                          GDALRegisterPlugin()                        */
/*                                                                      */
/*      Register a plugin by name, returning an error if not found      */
/************************************************************************/

/**
 * \brief Register a plugin by name, returning an error if not found
 *
 * This function will call GDALDriverManager::LoadPlugin() to register a
 * specific plugin by name.
 *
 * This method is intended to be called instead of GDALAllRegister() or
 * GDALRegisterPlugins() when fine tuning which drivers are needed at runtime.
 *
 * @see GDALDriverManager::LoadPlugin()
 * @see GDALDriverManager::AutoLoadDrivers()
 * @since GDAL 3.8
*/
CPLErr GDALRegisterPlugin(const char *name)
{
    auto poDriverManager = GetGDALDriverManager();
    // LoadPlugin is a no-op if compiled with GDAL_NO_AUTOLOAD defined.
    return poDriverManager->LoadPlugin(name);
}

/************************************************************************/
/*                          GDALRegisterPlugins()                       */
/*                                                                      */
/*      Register drivers and support code available as a plugin.        */
/************************************************************************/

/**
 * \brief Register drivers and support code available as a plugin.
 *
 * This function will call GDALDriverManager::AutoLoadDrivers() to
 * register all drivers or supporting code (for example VRT pixelfunctions
 * or VSI adapters) that have not been compiled into the GDAL core but instead
 * are made available through GDAL's plugin mechanism.
 *
 * This method is intended to be called instead of GDALAllRegister() when
 * fine tuning which drivers are needed at runtime.
 *
 * @see GDALDriverManager::AutoLoadDrivers()
 * @since GDAL 3.8
*/
void CPL_DLL GDALRegisterPlugins(void)
{
    auto poDriverManager = GetGDALDriverManager();
    // AutoLoadDrivers is a no-op if compiled with GDAL_NO_AUTOLOAD defined.
    poDriverManager->AutoLoadDrivers();
    poDriverManager->AutoLoadPythonDrivers();

    /* -------------------------------------------------------------------- */
    /*      Deregister any drivers explicitly marked as suppressed by the   */
    /*      GDAL_SKIP environment variable.                                 */
    /* -------------------------------------------------------------------- */
    poDriverManager->AutoSkipDrivers();

    poDriverManager->ReorderDrivers();
}

/************************************************************************/
/*                          GDALAllRegister()                           */
/*                                                                      */
/*      Register all identifiably supported formats.                    */
/************************************************************************/

/**
 * Register all known configured GDAL drivers.
 *
 * This function will drive any of the following that are configured into
 * GDAL.  See <a href="http://gdal.org/formats_list.html">raster list</a> and
 * <a href="http://gdal.org/ogr_formats.html">vector full list</a>
 *
 * This function should generally be called once at the beginning of the
 * application.
 */

void CPL_STDCALL GDALAllRegister()

{
    auto poDriverManager = GetGDALDriverManager();

#if defined(HAVE_EXTERNAL_DEFERRED_PLUGINS)
    DeclareExternalDeferredPlugins();
#endif

#if defined(DEFERRED_ARROW_DRIVER)
    DeclareDeferredOGRArrowPlugin();
#endif
#if defined(DEFERRED_BASISU_KTX2_DRIVER)
    DeclareDeferredBASISU_KTX2Plugin();
#endif
#if defined(DEFERRED_CAD_DRIVER)
    DeclareDeferredOGRCADPlugin();
#endif
#if defined(DEFERRED_CARTO_DRIVER)
    DeclareDeferredOGRCartoPlugin();
#endif
#if defined(DEFERRED_DDS_DRIVER)
    DeclareDeferredDDSPlugin();
#endif
#if defined(DEFERRED_DWG_DRIVER)
    DeclareDeferredOGRDWGPlugin();
    DeclareDeferredOGRDGNV8Plugin();
#endif
#if defined(DEFERRED_ELASTIC_DRIVER)
    DeclareDeferredOGRElasticPlugin();
#endif
#if defined(DEFERRED_EXR_DRIVER)
    DeclareDeferredEXRPlugin();
#endif
#if defined(DEFERRED_ECW_DRIVER)
    DeclareDeferredECWPlugin();
#endif
#if defined(DEFERRED_FILEGDB_DRIVER)
    DeclareDeferredOGRFileGDBPlugin();
#endif
#if defined(DEFERRED_FITS_DRIVER)
    DeclareDeferredFITSPlugin();
#endif
#if defined(DEFERRED_GEOR_DRIVER)
    DeclareDeferredGEORPlugin();
#endif
#if defined(DEFERRED_GIF_DRIVER)
    DeclareDeferredGIFPlugin();
#endif
#if defined(DEFERRED_GMLAS_DRIVER)
    DeclareDeferredOGRGMLASPlugin();
#endif
#if defined(DEFERRED_GRIB_DRIVER)
    DeclareDeferredGRIBPlugin();
#endif
#if defined(DEFERRED_GTA_DRIVER)
    DeclareDeferredGTAPlugin();
#endif
#if defined(DEFERRED_HANA_DRIVER)
    DeclareDeferredOGRHANAPlugin();
#endif
#if defined(DEFERRED_AVIF_DRIVER)
    DeclareDeferredAVIFPlugin();
#endif
#if defined(DEFERRED_HEIF_DRIVER)
    DeclareDeferredHEIFPlugin();
#endif
#if defined(DEFERRED_HDF4_DRIVER)
    DeclareDeferredHDF4Plugin();
#endif
#if defined(DEFERRED_KEA_DRIVER)
    // Must be registered before HDF5 so that when the plugin is not
    // installer the proper suggestion message is displayed
    DeclareDeferredKEAPlugin();
#endif
#if defined(DEFERRED_HDF5_DRIVER)
    DeclareDeferredHDF5Plugin();
#endif
#if defined(DEFERRED_IDB_DRIVER)
    DeclareDeferredOGRIDBPlugin();
#endif
#if defined(DEFERRED_JP2KAK_DRIVER)
    DeclareDeferredJP2KAKPlugin();
#endif
#if defined(DEFERRED_JP2OPENJPEG_DRIVER)
    DeclareDeferredOPENJPEGPlugin();
#endif
#if defined(DEFERRED_JPEG_DRIVER)
    DeclareDeferredJPEGPlugin();
#endif
#if defined(DEFERRED_JPEGXL_DRIVER)
    DeclareDeferredJPEGXLPlugin();
#endif
#if defined(DEFERRED_JPIPKAK_DRIVER)
    DeclareDeferredJPIPKAKPlugin();
#endif
#if defined(DEFERRED_LIBKML_DRIVER)
    DeclareDeferredOGRLIBKMLPlugin();
#endif
#if defined(DEFERRED_MONGODBV3_DRIVER)
    DeclareDeferredOGRMongoDBv3Plugin();
#endif
#if defined(DEFERRED_MRF_DRIVER)
    DeclareDeferredMRFPlugin();
#endif
#if defined(DEFERRED_MRSID_DRIVER)
    DeclareDeferredMrSIDPlugin();
#endif
#if defined(DEFERRED_MSG_DRIVER)
    DeclareDeferredMSGPlugin();
#endif
#if defined(DEFERRED_MSSQLSPATIAL_DRIVER)
    DeclareDeferredOGRMSSQLSpatialPlugin();
#endif
#if defined(DEFERRED_MYSQL_DRIVER)
    DeclareDeferredOGRMySQLPlugin();
#endif
#if defined(DEFERRED_NETCDF_DRIVER)
    DeclareDeferredNetCDFPlugin();
#endif
#if defined(DEFERRED_NITF_DRIVER)
    DeclareDeferredNITFPlugin();
#endif
#if defined(DEFERRED_OCI_DRIVER)
    DeclareDeferredOGROCIPlugin();
#endif
#if defined(DEFERRED_ODBC_DRIVER)
    DeclareDeferredOGRODBCPlugin();
#endif
#if defined(DEFERRED_OGDI_DRIVER)
    DeclareDeferredOGROGDIPlugin();
#endif
#if defined(DEFERRED_OPENFILEGDB_DRIVER)
    DeclareDeferredOGROpenFileGDBPlugin();
#endif
#if defined(DEFERRED_PARQUET_DRIVER)
    DeclareDeferredOGRParquetPlugin();
#endif
#if defined(DEFERRED_PCIDSK_DRIVER)
    DeclareDeferredPCIDSKPlugin();
#endif
#if defined(DEFERRED_PCRASTER_DRIVER)
    DeclareDeferredPCRasterPlugin();
#endif
#if defined(DEFERRED_POSTGISRASTER_DRIVER)
    DeclareDeferredPostGISRasterPlugin();
#endif
#if defined(DEFERRED_PLSCENES_DRIVER)
    DeclareDeferredOGRPLSCENESPlugin();
#endif
#if defined(DEFERRED_PDF_DRIVER)
    DeclareDeferredPDFPlugin();
#endif
#if defined(DEFERRED_PDS_DRIVER)
    DeclareDeferredPDSPlugin();
#endif
#if defined(DEFERRED_PG_DRIVER)
    DeclareDeferredOGRPGPlugin();
#endif
#if defined(DEFERRED_PNG_DRIVER)
    DeclareDeferredPNGPlugin();
#endif
#if defined(DEFERRED_RASTERLITE_DRIVER)
    DeclareDeferredRasterlitePlugin();
#endif
#if defined(DEFERRED_SOSI_DRIVER)
    DeclareDeferredOGRSOSIPlugin();
#endif
#if defined(DEFERRED_TILEDB_DRIVER)
    DeclareDeferredTileDBPlugin();
#endif
#if defined(DEFERRED_VFK_DRIVER)
    DeclareDeferredOGRVFKPlugin();
#endif
#if defined(DEFERRED_WCS_DRIVER)
    DeclareDeferredWCSPlugin();
#endif
#if defined(DEFERRED_WEBP_DRIVER)
    DeclareDeferredWEBPPlugin();
#endif
#if defined(DEFERRED_WMS_DRIVER)
    DeclareDeferredWMSPlugin();
#endif
#if defined(DEFERRED_WMTS_DRIVER)
    DeclareDeferredWMTSPlugin();
#endif
#if defined(DEFERRED_XLS_DRIVER)
    DeclareDeferredOGRXLSPlugin();
#endif
#if defined(DEFERRED_ZARR_DRIVER)
    DeclareDeferredZarrPlugin();
#endif
#if defined(DEFERRED_XODR_DRIVER)
    DeclareDeferredOGRXODRPlugin();
#endif
#if defined(DEFERRED_ADBC_DRIVER)
    DeclareDeferredOGRADBCPlugin();
#endif

    // AutoLoadDrivers is a no-op if compiled with GDAL_NO_AUTOLOAD defined.
    poDriverManager->AutoLoadDrivers();

    // NOTE: frmts/drivers.ini in the same directory should be kept in same
    // order as this file

#ifdef FRMT_vrt
    GDALRegister_VRT();
#endif

#ifdef FRMT_derived
    GDALRegister_Derived();
#endif

#ifdef FRMT_gti
    GDALRegister_GTI();
#endif

#ifdef FRMT_snap_tiff
    GDALRegister_SNAP_TIFF();
#endif

#ifdef FRMT_gtiff
    GDALRegister_GTiff();
    GDALRegister_COG();
#endif

#ifdef FRMT_libertiff
    GDALRegister_LIBERTIFF();
#endif

#ifdef FRMT_nitf
    GDALRegister_NITF();
    GDALRegister_RPFTOC();
    GDALRegister_ECRGTOC();
#endif

#ifdef FRMT_hfa
    GDALRegister_HFA();
#endif

#ifdef FRMT_ceos2
    GDALRegister_SAR_CEOS();
#endif

#ifdef FRMT_ceos
    GDALRegister_CEOS();
#endif

#ifdef FRMT_jaxapalsar
    GDALRegister_PALSARJaxa();
#endif

#ifdef FRMT_gff
    GDALRegister_GFF();
#endif

#ifdef FRMT_esric
    GDALRegister_ESRIC();
#endif

#ifdef FRMT_aigrid
    //    GDALRegister_AIGrid2();
    GDALRegister_AIGrid();
#endif

#ifdef FRMT_aaigrid
    GDALRegister_AAIGrid();
    GDALRegister_GRASSASCIIGrid();
    GDALRegister_ISG();
#endif

#ifdef FRMT_dted
    GDALRegister_DTED();
#endif

#ifdef FRMT_png
    GDALRegister_PNG();
#endif

#ifdef FRMT_dds
    GDALRegister_DDS();
#endif

#ifdef FRMT_gta
    GDALRegister_GTA();
#endif

#ifdef FRMT_jpeg
    GDALRegister_JPEG();
#endif

#ifdef FRMT_mem
    GDALRegister_MEM();
#endif

#ifdef FRMT_jdem
    GDALRegister_JDEM();
#endif

#ifdef FRMT_gif
    GDALRegister_GIF();
    GDALRegister_BIGGIF();
#endif

#ifdef FRMT_envisat
    GDALRegister_Envisat();
#endif

#ifdef FRMT_fits
    GDALRegister_FITS();
#endif

#ifdef FRMT_bsb
    GDALRegister_BSB();
#endif

#ifdef FRMT_bmp
    GDALRegister_BMP();
#endif

#ifdef FRMT_dimap
    GDALRegister_DIMAP();
#endif

#ifdef FRMT_airsar
    GDALRegister_AirSAR();
#endif

#ifdef FRMT_rs2
    GDALRegister_RS2();
#endif

#ifdef FRMT_safe
    GDALRegister_SAFE();
#endif

#ifdef FRMT_pcidsk
    GDALRegister_PCIDSK();
#endif

#ifdef FRMT_pcraster
    GDALRegister_PCRaster();
#endif

#ifdef FRMT_ilwis
    GDALRegister_ILWIS();
#endif

#ifdef FRMT_srtmhgt
    GDALRegister_SRTMHGT();
#endif

#ifdef FRMT_leveller
    GDALRegister_Leveller();
#endif

#ifdef FRMT_terragen
    GDALRegister_Terragen();
#endif

#ifdef FRMT_netcdf
    GDALRegister_netCDF();
#endif

#ifdef FRMT_hdf4
    GDALRegister_HDF4();
    GDALRegister_HDF4Image();
#endif

#ifdef FRMT_pds
    GDALRegister_ISIS3();
    GDALRegister_ISIS2();
    GDALRegister_PDS();
    GDALRegister_PDS4();
    GDALRegister_VICAR();
#endif

#ifdef FRMT_til
    GDALRegister_TIL();
#endif

#ifdef FRMT_ers
    GDALRegister_ERS();
#endif

#ifdef FRMT_jp2kak
    // JPEG2000 support using Kakadu toolkit
    GDALRegister_JP2KAK();
#endif

#ifdef FRMT_jpipkak
    // JPEG2000 support using Kakadu toolkit
    GDALRegister_JPIPKAK();
#endif

#ifdef FRMT_ecw
    GDALRegister_ECW();
    GDALRegister_JP2ECW();
#endif

#ifdef FRMT_openjpeg
    // JPEG2000 support using OpenJPEG library
    GDALRegister_JP2OpenJPEG();
#endif

#ifdef FRMT_l1b
    GDALRegister_L1B();
#endif

#ifdef FRMT_grib
    GDALRegister_GRIB();
#endif

#ifdef FRMT_mrsid
    GDALRegister_MrSID();
#endif

#ifdef FRMT_rmf
    GDALRegister_RMF();
#endif

#ifdef FRMT_wcs
    GDALRegister_WCS();
#endif

#ifdef FRMT_wms
    GDALRegister_WMS();
#endif

#ifdef FRMT_msgn
    GDALRegister_MSGN();
#endif

#ifdef FRMT_msg
    GDALRegister_MSG();
#endif

#ifdef FRMT_idrisi
    GDALRegister_IDRISI();
#endif

#ifdef FRMT_gsg
    GDALRegister_GSAG();
    GDALRegister_GSBG();
    GDALRegister_GS7BG();
#endif

#ifdef FRMT_cosar
    GDALRegister_COSAR();
#endif

#ifdef FRMT_tsx
    GDALRegister_TSX();
#endif

#ifdef FRMT_coasp
    GDALRegister_COASP();
#endif

#ifdef FRMT_map
    GDALRegister_MAP();
#endif

#ifdef FRMT_kmlsuperoverlay
    GDALRegister_KMLSUPEROVERLAY();
#endif

#ifdef FRMT_webp
    GDALRegister_WEBP();
#endif

#ifdef FRMT_pdf
    GDALRegister_PDF();
#endif

#ifdef FRMT_mbtiles
    GDALRegister_MBTiles();
#endif

#ifdef FRMT_plmosaic
    GDALRegister_PLMOSAIC();
#endif

#ifdef FRMT_cals
    GDALRegister_CALS();
#endif

#ifdef FRMT_wmts
    GDALRegister_WMTS();
#endif

#ifdef FRMT_sentinel2
    GDALRegister_SENTINEL2();
#endif

#ifdef FRMT_mrf
    GDALRegister_mrf();
#endif

#ifdef FRMT_tiledb
    GDALRegister_TileDB();
#endif

#ifdef FRMT_rdb
    GDALRegister_RDB();
#endif
    /* -------------------------------------------------------------------- */
    /*      Put raw formats at the end of the list. These drivers support   */
    /*      various ASCII-header labeled formats, so the driver could be    */
    /*      confused if you have files in some of above formats and such    */
    /*      ASCII-header in the same directory.                             */
    /* -------------------------------------------------------------------- */

#ifdef FRMT_raw
    GDALRegister_raw_no_sidecar();
#endif

    /* -------------------------------------------------------------------- */
    /*      Our test for the following is weak or expensive so we try       */
    /*      them last.                                                      */
    /* -------------------------------------------------------------------- */

#ifdef FRMT_rik
    GDALRegister_RIK();
#endif

#ifdef FRMT_usgsdem
    GDALRegister_USGSDEM();
#endif

#ifdef FRMT_gxf
    GDALRegister_GXF();
#endif

/* Register KEA before HDF5 */
#ifdef FRMT_kea
    GDALRegister_KEA();
#endif

#ifdef FRMT_hdf5
    GDALRegister_BAG();
    GDALRegister_S102();
    GDALRegister_S104();
    GDALRegister_S111();
    GDALRegister_HDF5();
    GDALRegister_HDF5Image();
#endif

#ifdef FRMT_northwood
    GDALRegister_NWT_GRD();
    GDALRegister_NWT_GRC();
#endif

#ifdef FRMT_adrg
    GDALRegister_ADRG();
    GDALRegister_SRP();
#endif

#ifdef FRMT_georaster
    GDALRegister_GEOR();
#endif

#ifdef FRMT_postgisraster
    GDALRegister_PostGISRaster();
#endif

#ifdef FRMT_saga
    GDALRegister_SAGA();
#endif

#ifdef FRMT_xyz
    GDALRegister_XYZ();
#endif

#ifdef FRMT_hf2
    GDALRegister_HF2();
#endif

#ifdef FRMT_ctg
    GDALRegister_CTG();
#endif

#ifdef FRMT_zmap
    GDALRegister_ZMap();
#endif

#ifdef FRMT_ngsgeoid
    GDALRegister_NGSGEOID();
#endif

#ifdef FRMT_iris
    GDALRegister_IRIS();
#endif

#ifdef FRMT_prf
    GDALRegister_PRF();
#endif

#ifdef FRMT_eeda
    GDALRegister_EEDAI();
    GDALRegister_EEDA();
#endif

#ifdef FRMT_daas
    GDALRegister_DAAS();
#endif

#ifdef FRMT_null
    GDALRegister_NULL();
#endif

#ifdef FRMT_sigdem
    GDALRegister_SIGDEM();
#endif

#ifdef FRMT_exr
    GDALRegister_EXR();
#endif

#ifdef FRMT_avif
    GDALRegister_AVIF();
#endif

#ifdef FRMT_heif
    GDALRegister_HEIF();
#endif

#ifdef FRMT_tga
    GDALRegister_TGA();
#endif

#ifdef FRMT_ogcapi
    GDALRegister_OGCAPI();
#endif

#ifdef FRMT_stacta
    GDALRegister_STACTA();
#endif

#ifdef FRMT_stacit
    GDALRegister_STACIT();
#endif

#ifdef FRMT_jpegxl
    GDALRegister_JPEGXL();
#endif

#ifdef FRMT_basisu_ktx2
    GDALRegister_BASISU();
    GDALRegister_KTX2();
#endif

#ifdef FRMT_gdalg
    GDALRegister_GDALG();
#endif

    // NOTE: you need to generally insert your own driver before that line.

    // NOTE: frmts/drivers.ini in the same directory should be kept in same
    // order as this file

/* -------------------------------------------------------------------- */
/*     GNM and OGR drivers                                              */
/* -------------------------------------------------------------------- */
#ifdef GNM_ENABLED
    GNMRegisterAllInternal();
#endif

    OGRRegisterAllInternal();

    /* -------------------------------------------------------------------- */
    /*      Put here drivers that absolutely need to look for side car      */
    /*      files in their Identify()/Open() procedure.                     */
    /* -------------------------------------------------------------------- */

#ifdef FRMT_raw
    GDALRegister_raw_with_sidecar();
#endif

#ifdef FRMT_zarr
    GDALRegister_Zarr();
#endif

#ifdef FRMT_rcm
    GDALRegister_RCM();
#endif

/* -------------------------------------------------------------------- */
/*      Register GDAL HTTP last, to let a chance to other drivers       */
/*      accepting URL to handle them before.                            */
/* -------------------------------------------------------------------- */
#ifdef FRMT_http
    GDALRegister_HTTP();
#endif

    poDriverManager->AutoLoadPythonDrivers();

    /* -------------------------------------------------------------------- */
    /*      Deregister any drivers explicitly marked as suppressed by the   */
    /*      GDAL_SKIP environment variable.                                 */
    /* -------------------------------------------------------------------- */
    poDriverManager->AutoSkipDrivers();

    poDriverManager->ReorderDrivers();
}
