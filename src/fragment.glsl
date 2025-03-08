#version 300 es
precision mediump float;
out vec4 fragcolor;
uniform vec2 u_resolution;
uniform float u_zoom;  // Zoom level (e.g., 1.0 for normal, 100.0 for deep zoom)
uniform vec2 u_offset; // Offset for panning
uniform int u_max_iter;
uniform float u_time;

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution) / u_resolution.y;
    vec2 c = uv / u_zoom + u_offset;  // Adjust based on zoom and offset
    vec2 z = vec2(0.0);

    // int maxIter = int(50.0 + log(u_zoom) * 20.0); // Dynamic iterations

    int i;
    for (i = 0; i < u_max_iter; i++) {
        // if (dot(z, z) > 4.0) break;
        if (dot(z, z) > 16.0) break;
        // z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        z = vec2(z.x * z.x - z.y * z.y, (2.0 + sin(u_time)) * z.x * z.y) + c;
    }

    float t = float(i) / float(u_max_iter);
    
    // fragcolor = vec4(t, t * t, t * t * t, 1.0);
    // if (t >= 0.99) fragcolor = vec4(1.0, 1.0, 1.0, 1.0);
    // else fragcolor = vec4(0.0, 0.0, 0.0, 1.0);
    if (i % 2 == 0) fragcolor = vec4(1.0, 1.0, 1.0, 1.0);
    else fragcolor = vec4(0.0, 0.0, 0.0, 1.0);
}


// #version 300 es
// precision highp float;
// out vec4 fragcolor;

// uniform highp vec2 u_resolution;
// uniform highp float u_zoom;
// uniform highp vec2 u_offset;
// uniform int u_maxIter; // For deep zooms, try increasing this beyond 1000

// ////////////////////////
// // Double-double arithmetic helper functions

// // Split a float into high and low parts
// vec2 splitFloat(float a) {
//     float c = 4097.0 * a; // (2^12 + 1)
//     float a_hi = c - (c - a);
//     float a_lo = a - a_hi;
//     return vec2(a_hi, a_lo);
// }

// // Two-product: returns product and error term as a double-double
// vec2 twoProd(float a, float b) {
//     float p = a * b;
//     vec2 a_split = splitFloat(a);
//     vec2 b_split = splitFloat(b);
//     float err = a_split.y * b_split.y -
//                 (((p - a_split.x * b_split.x) - a_split.y * b_split.x) - a_split.x * b_split.y);
//     return vec2(p, err);
// }

// // Two-sum: returns sum and error term as a double-double
// vec2 twoSum(float a, float b) {
//     float s = a + b;
//     float bp = s - a;
//     float err = (a - (s - bp)) + (b - bp);
//     return vec2(s, err);
// }

// // Add two double-doubles (each stored in a vec2: (hi, lo))
// vec2 dd_add(vec2 a, vec2 b) {
//     vec2 s = twoSum(a.x, b.x);
//     float err = a.y + b.y + s.y;
//     vec2 r = twoSum(s.x, err);
//     return r;
// }

// // Subtract two double-doubles: a - b
// vec2 dd_sub(vec2 a, vec2 b) {
//     vec2 s = twoSum(a.x, -b.x);
//     float err = a.y - b.y + s.y;
//     vec2 r = twoSum(s.x, err);
//     return r;
// }

// // Multiply two double-doubles: a * b
// vec2 dd_mul(vec2 a, vec2 b) {
//     vec2 p = twoProd(a.x, b.x);
//     float err = a.x * b.y + a.y * b.x;
//     vec2 s = twoSum(p.x, err);
//     return vec2(s.x, s.y + p.y);
// }

// // Convert a float to a double-double (with zero error part)
// vec2 to_dd(float a) {
//     return vec2(a, 0.0);
// }

// ////////////////////////
// // Complex number in double-double arithmetic
// struct dcomplex {
//     vec2 re; // Real part as a double-double (hi, lo)
//     vec2 im; // Imaginary part as a double-double (hi, lo)
// };

// // Complex addition: a + b
// dcomplex dc_add(dcomplex a, dcomplex b) {
//     dcomplex r;
//     r.re = dd_add(a.re, b.re);
//     r.im = dd_add(a.im, b.im);
//     return r;
// }

// // Complex multiplication: a * b
// dcomplex dc_mul(dcomplex a, dcomplex b) {
//     dcomplex r;
//     // r.re = a.re * b.re - a.im * b.im
//     r.re = dd_sub(dd_mul(a.re, b.re), dd_mul(a.im, b.im));
//     // r.im = a.re * b.im + a.im * b.re
//     r.im = dd_add(dd_mul(a.re, b.im), dd_mul(a.im, b.re));
//     return r;
// }

// // Complex square: a^2
// dcomplex dc_sqr(dcomplex a) {
//     return dc_mul(a, a);
// }

// // Compute magnitude squared of a complex number: |a|^2 = a.re^2 + a.im^2
// vec2 dc_mag2(dcomplex a) {
//     vec2 re2 = dd_mul(a.re, a.re);
//     vec2 im2 = dd_mul(a.im, a.im);
//     return dd_add(re2, im2);
// }

// ////////////////////////
// // Mandelbrot iteration using double-double arithmetic
// float mandelbrot(dcomplex c, int maxIter) {
//     dcomplex z;
//     z.re = to_dd(0.0);
//     z.im = to_dd(0.0);
//     int i;
//     for (i = 0; i < maxIter; i++) {
//         vec2 mag2 = dc_mag2(z);
//         if (mag2.x > 4.0) break;
//         z = dc_add(dc_sqr(z), c);
//     }
//     return float(i);
// }

// ////////////////////////
// // Simple color mapping function
// vec3 getColor(float iter, float maxIter) {
//     float t = iter / maxIter;
//     return vec3(t, t*t, t*t*t);
// }

// ////////////////////////
// // Main shader entry
// void main() {
//     // First, compute normalized screen coordinates using gl_FragCoord.
//     // We'll convert these to double-double numbers.
//     float normX = (gl_FragCoord.x - 0.5 * u_resolution.x) / u_resolution.y;
//     float normY = (gl_FragCoord.y - 0.5 * u_resolution.y) / u_resolution.y;
    
//     // Convert normalized coordinates into double-double format.
//     vec2 dd_normX = to_dd(normX);
//     vec2 dd_normY = to_dd(normY);
    
//     // Create a dcomplex for the uv coordinate.
//     dcomplex uv_dd;
//     uv_dd.re = dd_normX;
//     uv_dd.im = dd_normY;
    
//     // Instead of computing (uv / u_zoom) in float, compute it using double-double.
//     // We compute the reciprocal of u_zoom in float and convert it.
//     float invZoom = 1.0 / u_zoom;
//     vec2 dd_invZoom = to_dd(invZoom);
    
//     // Scale the uv coordinate by dd_invZoom.
//     vec2 c_re_dd = dd_mul(uv_dd.re, dd_invZoom);
//     vec2 c_im_dd = dd_mul(uv_dd.im, dd_invZoom);
    
//     // Add the offset (converted to double-double) to get the final complex coordinate c.
//     dcomplex c;
//     c.re = dd_add(c_re_dd, to_dd(u_offset.x));
//     c.im = dd_add(c_im_dd, to_dd(u_offset.y));
    
//     // Run the Mandelbrot iteration using double-double arithmetic.
//     float iter = mandelbrot(c, u_maxIter);
    
//     // Map iteration count to color.
//     vec3 color = getColor(iter, float(u_maxIter));
//     fragcolor = vec4(color, 1.0);
// }