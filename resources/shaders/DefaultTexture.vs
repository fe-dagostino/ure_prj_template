#version 100

precision mediump float;

uniform   mat4 u_m4MVP;
attribute vec2 a_v2Point;
attribute vec2 a_v2TexCoord;
varying   vec2 v_v2TexCoord;

void main()
{
  gl_Position  = u_m4MVP * vec4( a_v2Point, 0.0, 1.0 );
  v_v2TexCoord = a_v2TexCoord;
}

