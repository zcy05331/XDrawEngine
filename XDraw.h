#pragma once
#include "cairo-win32.h"
typedef class XDrawBase {
private:
	cairo_t* context;
	cairo_surface_t* surface;
public:
	XDrawBase() {
		context = nullptr;
		surface = nullptr;
	}
	~XDrawBase() {
		this->Destroy();
	}
	void Destroy() {
		cairo_destroy(context);
		cairo_surface_destroy(surface);
		cairo_path_destroy(this->Path());
	}
	XDrawBase operator=(XDrawBase x) {
		this->context = x.context;
		this->surface = x.surface;
		return *this;
	}
	inline cairo_t* Context() { return context; }
	inline cairo_surface_t* Surface() { return surface; }
	inline cairo_path_t* Path() { return cairo_copy_path(context); }
	inline cairo_path_t* PathFlat() { return cairo_copy_path_flat(context); }
	inline void CreateSurface(HDC hdc) { surface = cairo_win32_surface_create(hdc); }
	inline void CreateSurfacePNG(const char* filename) {
		surface = cairo_image_surface_create_from_png(filename);
	}
	inline void BindSurface() { context = cairo_create(surface); }
	inline void NewFromHDC(HDC hdc) {
		this->CreateSurface(hdc);
		this->BindSurface();
	}
	inline void Mask(double x, double y) {
		cairo_mask_surface(context, surface, x, y);
	}
	inline void SetSourceColor(double r, double g, double b, double a) { cairo_set_source_rgba(context, r, g, b, a); }
	inline void SetLineWidth(double width) { cairo_set_line_width(context, width); }
	inline void SetAntialiasing(cairo_antialias_t level) { cairo_set_antialias(context, level); }
	inline void Paint(double alpha = 1.0f) { cairo_paint_with_alpha(context, alpha); }
	inline void Fill() { cairo_fill(context); }
	inline void FillExtents(double* left, double* top, double* right, double* bottom) {
		cairo_fill_extents(context, left, top, right, bottom);
	}
	inline void Stroke() { cairo_stroke(context); }
	inline void Rectangle(double x, double y, double width, double height) {
		cairo_rectangle(context, x, y, width, height);
	}
	inline void SetSource(cairo_pattern_t* pattern) {
		cairo_set_source(context, pattern);
	}
	inline void SetMask(cairo_pattern_t* pattern) {
		cairo_mask(context, pattern);
	}
	inline void AddPath(const cairo_path_t* path) {
		cairo_append_path(context, path);
	}
	inline bool PathPoint(double* x, double* y) {
		if (!cairo_has_current_point(context))return false;
		cairo_get_current_point(context, x, y);
	}
	inline void NewPath() {
		cairo_new_path(context);
	}
	inline void NewSubPath() {
		cairo_new_sub_path(context);
	}
	inline void ClosePath() {
		cairo_close_path(context);
	}
	inline void Arc(double x, double y, double radius, double angle1, double angle2) {
		if (angle1 <= angle2)cairo_arc(context, x, y, radius, angle1, angle2);
		else cairo_arc_negative(context, x, y, radius, angle1, angle2);
	}
	inline void CurveTo(double x0, double y0, double x1, double y1, double x2, double y2) {
		cairo_curve_to(context, x0, y0, x1, y1, x2, y2);
	}
	inline void LineTo(double x, double y) {
		cairo_line_to(context, x, y);
	}
	inline void MoveTo(double x, double y) {
		cairo_move_to(context, x, y);
	}
	inline void FontFace(const char* family, cairo_font_slant_t slant, cairo_font_weight_t weight) {
		cairo_select_font_face(context, family, slant, weight);
	}
	inline void FontSize(double size) {
		cairo_set_font_size(context, size);
	}
	inline void FontTransform(const cairo_matrix_t* matrix) {
		cairo_set_font_matrix(context, matrix);
	}
	inline void FontOptions(const cairo_font_options_t* options) {
		cairo_set_font_options(context, options);
	}
	inline void ShowText(const char* str) {
		cairo_show_text(context, str);
	}
}*LPXDrawBase;

typedef class XDrawPattern {
private:
	LPXDrawBase base;
	cairo_pattern_t* pattern;
public:
	XDrawPattern() {
		base = nullptr;
		pattern = nullptr;
	}
	~XDrawPattern() {
		this->Destroy();
	}
	inline void Destroy() {
		base->Destroy();
		cairo_pattern_destroy(pattern);
	}
	inline XDrawPattern operator=(XDrawPattern x) {
		base = x.base;
		this->pattern = x.pattern;
		return *this;
	}
	inline void Bind(LPXDrawBase src) {
		base = src;
	}
	inline cairo_pattern_t* Pattern() { return pattern; }
	inline void BindSurface(cairo_surface_t* surface) {
		cairo_pattern_create_for_surface(surface);
	}
	inline void CreateLinear(double xStart, double yStart, double xEnd, double yEnd) {
		pattern = cairo_pattern_create_linear(xStart, yStart, xEnd, yEnd);
	}//gradient pattern
	inline void AddColor(double offset,double r, double g, double b, double a) {
		cairo_pattern_add_color_stop_rgba(pattern, offset, r, g, b, a);
	}//for both radial and linear
	inline void CreateRadial(double x0, double y0, double radius0, double x1, double y1, double radius1) {
		pattern = cairo_pattern_create_radial(x0, y0, radius0, x1, y1, radius1);
	}
	inline void CreateRGBA(double r, double g, double b, double a) {
		pattern = cairo_pattern_create_rgba(r, g, b, a);
	}
	inline void SetSource(cairo_t* context) {
		cairo_set_source(context, pattern);
	}
	inline void SetSource() {
		cairo_set_source(base->Context(), pattern);
	}
	inline void SetSource(LPXDrawBase x) {
		cairo_set_source(x->Context(), pattern);
	}
	inline void Mask(cairo_t* context) {
		cairo_mask(context, pattern);
	}
	inline void Mask() {
		cairo_mask(base->Context(), pattern);
	}
	inline void Mask(LPXDrawBase x) {
		cairo_mask(x->Context(), pattern);
	}
	inline void SetFilter(cairo_filter_t filter) {
		cairo_pattern_set_filter(pattern, filter);
	}
}*LPXDrawPattern;