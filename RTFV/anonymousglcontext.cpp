#include <anonymousglcontext.hpp>

AnonymousGLContext::AnonymousGLContext(wxGLCanvas *canvas)
  : wxGLContext(canvas, NULL, NULL)
{
  bool happy = loadOBJ();
  std::cout << "Model was loaded? " << happy <<'\n';
  SetCurrent(*canvas);
  
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  GLfloat ambient[] = {0.75, 0.75, 0.75};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0f, 1.0f, -0.75f, 0.75f, 0.6f, 4.0f);

}

void AnonymousGLContext::DrawModel(float rx, float ry, float rz){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, -0.25f, -1.0f);
  glRotatef(rx, 1.0f, 0.0f, 0.0f);
  glRotatef(ry, 0.0f, 1.0f, 0.0f);
  glRotatef(rz, 0.0f, 0.0f, 1.0f);
  for(unsigned int i = 0;i<_faces.size();i++){
    glBegin(GL_QUADS);
    for(unsigned int j=0;j<4;j++){
      glNormal3f(_normals.at(i).at(j).at(0),_normals.at(i).at(j).at(1),_normals.at(i).at(j).at(2));
      glTexCoord2f(_uvs.at(i).at(j).at(0),_uvs.at(i).at(j).at(1));
      glVertex3f(_faces.at(i).at(j).at(0),_faces.at(i).at(j).at(1),_faces.at(i).at(j).at(2));
    }
    glEnd();
  }
  glFlush();

}

bool AnonymousGLContext::loadOBJ(){
  std::vector<unsigned int> vertexInds, uvInds, normalInds;
  std::vector<std::vector<float>> tmp_vertices;
  std::vector<std::vector<float>> tmp_uvs;
  std::vector<std::vector<float>> tmp_normals;
  std::vector<float[3]> tmp_normd;

  FILE * file = fopen("wingquads.obj","r");
  if(file == NULL){
    std::cout << "OBJ FILE WAS NOT FOUND!\n";
    return false;
  }

  while(true){
    char linehead[128];
    int res = fscanf(file,"%s",linehead);

    if(res==EOF)
      break;
    if(strcmp(linehead, "v") == 0){

      std::vector<float> vertex;
      float vx,vy,vz;
      fscanf(file,"%f %f %f\n",&vx,&vy,&vz );
      vertex.push_back(vx);
      vertex.push_back(vy);
      vertex.push_back(vz);
      tmp_vertices.push_back(vertex);
    }else if(strcmp(linehead, "vt") ==0){
      std::vector<float> uv;
      float ux,uy;
      fscanf(file,"%f %f",&ux,&uy);
      uv.push_back(ux);
      uv.push_back(uy);
      tmp_uvs.push_back(uv);
    }else if(strcmp(linehead,"vn")==0){
      std::vector<float> normal;
      float nx,ny,nz;
      fscanf(file,"%f %f %f\n",&nx,&ny,&nz );
      normal.push_back(nx);
      normal.push_back(ny);
      normal.push_back(nz);
      tmp_normals.push_back(normal);
    }else if(strcmp(linehead,"f")==0){
      unsigned int vertexInd[4], uvInd[4] ,normalInd[4];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexInd[0], &uvInd[0], &normalInd[0],&vertexInd[1], &uvInd[1], &normalInd[1],&vertexInd[2], &uvInd[2], &normalInd[2],&vertexInd[3], &uvInd[3], &normalInd[3]);
      if(matches != 12){
        std::cout << "Error while parsing .obj file\n";
        return false;
      }

      std::vector<std::vector<float>> face;
      std::vector<std::vector<float>> uv;
      std::vector<std::vector<float>> nor;

      for(unsigned int i=0; i<4; i++){
        face.push_back(tmp_vertices.at(vertexInd[i]-1));
        uv.push_back(tmp_uvs.at(uvInd[i]-1));
        nor.push_back(tmp_normals.at(normalInd[i]-1));
      }
      _faces.push_back(face);
      _normals.push_back(nor);
      _uvs.push_back(uv);
    }
  }
  fclose(file);
  return true;
}


void AnonymousGLContext::DrawModelTris(float rx, float ry, float rz)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, -0.25f, -1.0f);
  glRotatef(rx, 1.0f, 0.0f, 0.0f);
  glRotatef(ry, 0.0f, 1.0f, 0.0f);
  glRotatef(rz, 0.0f, 0.0f, 1.0f);
  for(unsigned int i = 0;i<_faces.size();i++){
    glBegin(GL_TRIANGLES);
    for(unsigned int j=0;j<3;j++){
      glNormal3f(_normals.at(i).at(j).at(0),_normals.at(i).at(j).at(1),_normals.at(i).at(j).at(2));
      glTexCoord2f(_uvs.at(i).at(j).at(0),_uvs.at(i).at(j).at(1));
      glVertex3f(_faces.at(i).at(j).at(0),_faces.at(i).at(j).at(1),_faces.at(i).at(j).at(2));
    }
    glEnd();
  }
  glFlush();
}
