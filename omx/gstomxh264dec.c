/*
 * Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
 *   Author: Sebastian Dröge <sebastian.droege@collabora.co.uk>, Collabora Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>

#include "gstomxh264dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_omx_h264_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_h264_dec_debug_category

/* prototypes */
static void gst_omx_h264_dec_finalize (GObject * object);
static gboolean gst_omx_h264_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoState * state);
static gboolean gst_omx_h264_dec_set_format (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoState * state);

enum
{
  PROP_0
};

/* class initialization */

#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_omx_h264_dec_debug_category, "omxh264dec", 0, \
      "debug category for gst-omx video decoder base class");

GST_BOILERPLATE_FULL (GstOMXH264Dec, gst_omx_h264_dec,
    GstOMXVideoDec, GST_TYPE_OMX_VIDEO_DEC, DEBUG_INIT);

static void
gst_omx_h264_dec_base_init (gpointer g_class)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX H.264 Video Decoder",
      "Codec/Decoder/Video",
      "Decode H.264 video streams",
      "Sebastian Dröge <sebastian.droege@collabora.co.uk>");
}

static void
gst_omx_h264_dec_class_init (GstOMXH264DecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstOMXVideoDecClass *videodec_class = GST_OMX_VIDEO_DEC_CLASS (klass);

  gobject_class->finalize = gst_omx_h264_dec_finalize;

  videodec_class->is_format_change =
      GST_DEBUG_FUNCPTR (gst_omx_h264_dec_is_format_change);
  videodec_class->set_format = GST_DEBUG_FUNCPTR (gst_omx_h264_dec_set_format);

  videodec_class->default_sink_template_caps = "video/x-h264, "
      "parsed=(boolean) true, "
      "alignment=(string)au, " "stream-format=(string) avc";
  videodec_class->default_src_template_caps = GST_VIDEO_CAPS_YUV ("I420");
}

static void
gst_omx_h264_dec_init (GstOMXH264Dec * self, GstOMXH264DecClass * klass)
{
}

static void
gst_omx_h264_dec_finalize (GObject * object)
{
  /* GstOMXH264Dec *self = GST_OMX_H264_DEC (object); */

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_omx_h264_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoState * state)
{
  return FALSE;
}

static gboolean
gst_omx_h264_dec_set_format (GstOMXVideoDec * dec, GstOMXPort * port,
    GstVideoState * state)
{
  gboolean ret;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;

  gst_omx_port_get_port_definition (port, &port_def);
  port_def.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
  ret = gst_omx_port_update_port_definition (port, &port_def);

  return ret;
}
