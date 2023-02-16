#version 100

precision mediump float;

varying   vec2      v_v2TexCoord;
uniform   sampler2D u_2dTexture;
uniform   vec4      u_v4Color;

void main()
{
  gl_FragColor = vec4(1, 1, 1, texture2D(u_2dTexture, v_v2TexCoord).a) * u_v4Color;
}

