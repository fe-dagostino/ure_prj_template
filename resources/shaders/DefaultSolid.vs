#version 100

precision mediump float;

uniform   mat4  u_m4MVP;
uniform   float u_fThickness;
attribute vec2  a_v2Point;

void main()
{
  gl_PointSize = u_fThickness;
  gl_Position  = u_m4MVP * vec4( a_v2Point, 0, 1.0 );
}
