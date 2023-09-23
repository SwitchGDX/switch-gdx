#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include "java/nio/ByteBuffer.h"
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Bitmap.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_SizeMetrics.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Face.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Stroker.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Glyph.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Library.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_GlyphSlot.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_GlyphMetrics.h>
#include <com/badlogic/gdx/graphics/g2d/freetype/FreeType_Size.h>


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

static jint lastError = 0;

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType_getLastErrorCode_R_int(jcontext ctx) {

    return lastError;
}

void SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Library_doneFreeType_long(jcontext ctx, jlong library) {

    FT_Done_FreeType((FT_Library)library);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Library_newMemoryFace_long_java_nio_ByteBuffer_int_int_R_long(jcontext ctx, jlong library, jobject data_object, jint dataSize, jint faceIndex) {
    auto data = (jbyte *)((java_nio_Buffer *)data_object)->F_address;

    FT_Face face = 0;
    FT_Error error = FT_New_Memory_Face((FT_Library)library, (const FT_Byte*)data, dataSize, faceIndex, &face);
    if(error) {
        lastError = error;
        return 0;
    }
    else return (jlong)face;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Library_strokerNew_long_R_long(jcontext ctx, jlong library) {

    FT_Stroker stroker;
    FT_Error error = FT_Stroker_New((FT_Library)library, &stroker);
    if(error) {
        lastError = error;
        return 0;
    }
    else return (jlong)stroker;
}

void SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_doneFace_long(jcontext ctx, jlong face) {

    FT_Done_Face((FT_Face)face);
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getFaceFlags_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->face_flags;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getStyleFlags_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->style_flags;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getNumGlyphs_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->num_glyphs;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getAscender_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->ascender;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getDescender_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->descender;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getHeight_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->height;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getMaxAdvanceWidth_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->max_advance_width;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getMaxAdvanceHeight_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->max_advance_height;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getUnderlinePosition_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->underline_position;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getUnderlineThickness_long_R_int(jcontext ctx, jlong face) {

    return ((FT_Face)face)->underline_thickness;
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_selectSize_long_int_R_boolean(jcontext ctx, jlong face, jint strike_index) {

    return !FT_Select_Size((FT_Face)face, strike_index);
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_setCharSize_long_int_int_int_int_R_boolean(jcontext ctx, jlong face, jint charWidth, jint charHeight, jint horzResolution, jint vertResolution) {

    return !FT_Set_Char_Size((FT_Face)face, charWidth, charHeight, horzResolution, vertResolution);
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_setPixelSizes_long_int_int_R_boolean(jcontext ctx, jlong face, jint pixelWidth, jint pixelHeight) {

    return !FT_Set_Pixel_Sizes((FT_Face)face, pixelWidth, pixelHeight);
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_loadGlyph_long_int_int_R_boolean(jcontext ctx, jlong face, jint glyphIndex, jint loadFlags) {

    return !FT_Load_Glyph((FT_Face)face, glyphIndex, loadFlags);
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_loadChar_long_int_int_R_boolean(jcontext ctx, jlong face, jint charCode, jint loadFlags) {

    return !FT_Load_Char((FT_Face)face, charCode, loadFlags);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getGlyph_long_R_long(jcontext ctx, jlong face) {

    return (jlong)((FT_Face)face)->glyph;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getSize_long_R_long(jcontext ctx, jlong face) {

    return (jlong)((FT_Face)face)->size;
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_hasKerning_long_R_boolean(jcontext ctx, jlong face) {

    return FT_HAS_KERNING(((FT_Face)face));
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getKerning_long_int_int_int_R_int(jcontext ctx, jlong face, jint leftGlyph, jint rightGlyph, jint kernMode) {

    FT_Vector kerning;
    FT_Error error = FT_Get_Kerning((FT_Face)face, leftGlyph, rightGlyph, kernMode, &kerning);
    if(error) return 0;
    return kerning.x;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Face_getCharIndex_long_int_R_int(jcontext ctx, jlong face, jint charCode) {

    return FT_Get_Char_Index((FT_Face)face, charCode);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Size_getMetrics_long_R_long(jcontext ctx, jlong address) {

    return (jlong)&((FT_Size)address)->metrics;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getXppem_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->x_ppem;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getYppem_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->y_ppem;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getXscale_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->x_scale;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getYscale_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->x_scale;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getAscender_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->ascender;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getDescender_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->descender;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getHeight_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->height;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$SizeMetrics_getMaxAdvance_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Size_Metrics*)metrics)->max_advance;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getMetrics_long_R_long(jcontext ctx, jlong slot) {

    return (jlong)&((FT_GlyphSlot)slot)->metrics;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getLinearHoriAdvance_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->linearHoriAdvance;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getLinearVertAdvance_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->linearVertAdvance;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getAdvanceX_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->advance.x;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getAdvanceY_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->advance.y;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getFormat_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->format;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getBitmap_long_R_long(jcontext ctx, jlong slot) {

    FT_GlyphSlot glyph = ((FT_GlyphSlot)slot);
    return (jlong)&(glyph->bitmap);
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getBitmapLeft_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->bitmap_left;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getBitmapTop_long_R_int(jcontext ctx, jlong slot) {

    return ((FT_GlyphSlot)slot)->bitmap_top;
}

jbool SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_renderGlyph_long_int_R_boolean(jcontext ctx, jlong slot, jint renderMode) {

    return !FT_Render_Glyph((FT_GlyphSlot)slot, (FT_Render_Mode)renderMode);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphSlot_getGlyph_long_R_long(jcontext ctx, jlong glyphSlot) {

    FT_Glyph glyph;
    FT_Error error = FT_Get_Glyph((FT_GlyphSlot)glyphSlot, &glyph);
    if(error) {
        lastError = error;
        return 0;
    }
    else return (jlong)glyph;
}

void SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_done_long(jcontext ctx, jlong glyph) {

    FT_Done_Glyph((FT_Glyph)glyph);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_strokeBorder_long_long_boolean_R_long(jcontext ctx, jlong glyph, jlong stroker, jbool inside) {

    FT_Glyph border_glyph = (FT_Glyph)glyph;
    FT_Glyph_StrokeBorder(&border_glyph, (FT_Stroker)stroker, inside, 1);
    return (jlong)border_glyph;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_toBitmap_long_int_R_long(jcontext ctx, jlong glyph, jint renderMode) {

    FT_Glyph bitmap = (FT_Glyph)glyph;
    FT_Error error = FT_Glyph_To_Bitmap(&bitmap, (FT_Render_Mode)renderMode, NULL, 1);
    if(error) {
        lastError = error;
        return 0;
    }
    return (jlong)bitmap;
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_getBitmap_long_R_long(jcontext ctx, jlong glyph) {

    FT_BitmapGlyph glyph_bitmap = ((FT_BitmapGlyph)glyph);
    return (jlong)&(glyph_bitmap->bitmap);
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_getLeft_long_R_int(jcontext ctx, jlong glyph) {

    FT_BitmapGlyph glyph_bitmap = ((FT_BitmapGlyph)glyph);
    return glyph_bitmap->left;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Glyph_getTop_long_R_int(jcontext ctx, jlong glyph) {

    FT_BitmapGlyph glyph_bitmap = ((FT_BitmapGlyph)glyph);
    return glyph_bitmap->top;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getRows_long_R_int(jcontext ctx, jlong bitmap) {

    return ((FT_Bitmap*)bitmap)->rows;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getWidth_long_R_int(jcontext ctx, jlong bitmap) {

    return ((FT_Bitmap*)bitmap)->width;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getPitch_long_R_int(jcontext ctx, jlong bitmap) {

    return ((FT_Bitmap*)bitmap)->pitch;
}

jobject SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getBuffer_long_R_java_nio_ByteBuffer(jcontext ctx, jlong bitmap) {

    auto bmp = (FT_Bitmap*)bitmap;
    auto buffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int_boolean(ctx, buffer, (jlong)bmp->buffer, (int)bmp->rows * abs(bmp->pitch), false);
    buffer->gcMark = GC_MARK_START;
    return buffer;
// 			return env->NewDirectByteBuffer((void*)bmp->buffer, bmp->rows * abs(bmp->pitch));
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getNumGray_long_R_int(jcontext ctx, jlong bitmap) {

    return ((FT_Bitmap*)bitmap)->num_grays;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Bitmap_getPixelMode_long_R_int(jcontext ctx, jlong bitmap) {

    return ((FT_Bitmap*)bitmap)->pixel_mode;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getWidth_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->width;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getHeight_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->height;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getHoriBearingX_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->horiBearingX;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getHoriBearingY_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->horiBearingY;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getHoriAdvance_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->horiAdvance;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getVertBearingX_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->vertBearingX;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getVertBearingY_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->vertBearingY;
}

jint SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$GlyphMetrics_getVertAdvance_long_R_int(jcontext ctx, jlong metrics) {

    return ((FT_Glyph_Metrics*)metrics)->vertAdvance;
}

void SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Stroker_set_long_int_int_int_int(jcontext ctx, jlong stroker, jint radius, jint lineCap, jint lineJoin, jint miterLimit) {

    FT_Stroker_Set((FT_Stroker)stroker, radius, (FT_Stroker_LineCap)lineCap, (FT_Stroker_LineJoin)lineJoin, miterLimit);
}

void SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType$Stroker_done_long(jcontext ctx, jlong stroker) {

    FT_Stroker_Done((FT_Stroker)stroker);
}

jlong SM_com_badlogic_gdx_graphics_g2d_freetype_FreeType_initFreeTypeJni_R_long(jcontext ctx) {

    FT_Library library = 0;
    FT_Error error = FT_Init_FreeType(&library);
    if(error) {
        lastError = error;
        return 0;
    }
    else return (jlong)library;
}

