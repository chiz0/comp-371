 #version 330 core\n 
     in vec3 vertexColor; 
     in vec2 vertexUV; 
     uniform sampler2D textureSampler; 
     uniform sampler2D textureSampler2; 
      
     out vec4 FragColor; 
     void main() 
     { 
        vec4 textureColor = texture( textureSampler, vertexUV ); 
         vec4 textureColor2 = texture( textureSampler2, vertexUV ); 
          
        FragColor = textureColor * textureColor2; 
     } 