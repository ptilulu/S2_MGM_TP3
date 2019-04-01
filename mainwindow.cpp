#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

/* **** début de la partie à compléter **** */

float MainWindow::faceArea(MyMesh* _mesh, int faceID)
{
    FaceHandle fh = _mesh->face_handle(faceID);
    std::vector <int> pointID;
    VectorT <float,3> points[3];

    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(fh); curVert.is_valid(); curVert ++)
    {
        pointID.push_back((*curVert).idx());
    }

    for (int i=0; i<pointID.size();i++)
    {
        VertexHandle vh = _mesh->vertex_handle(pointID.at(i));
        points[i][0] = _mesh->point(vh)[0];
        points[i][1] = _mesh->point(vh)[1];
        points[i][2] = _mesh->point(vh)[2];
    }
    VectorT<float,3> BmoinsA = points[1] - points[0];
    VectorT<float,3> CmoinsA = points[2] - points[0];

    VectorT<float,3> res;
    res[0] = BmoinsA[1] * CmoinsA[2] - BmoinsA[2] * CmoinsA[1];
    res[1] = BmoinsA[2] * CmoinsA[0] - BmoinsA[0] * CmoinsA[2];
    res[2] = BmoinsA[0] * CmoinsA[1] - BmoinsA[1] * CmoinsA[0];

    return res.norm()/2.0;
}

float MainWindow::angleFF(MyMesh* _mesh, int faceID0,  int faceID1, int vertID0, int vertID1)
{
    FaceHandle fh = _mesh->face_handle(faceID0);
    std::vector <int> pointID;
    int debut = -1;
    bool pos = false;

    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(fh); curVert.is_valid(); curVert ++)
    {
       pointID.push_back((*curVert).idx());
    }

    for(int i = 0; i < 3; i++){
        if(vertID0 == pointID.at(i))
            debut = i;
    }

    if(pointID.at((debut+2)%3) == vertID1)
        pos = true;

    // première face
    VectorT <float,6> vec = VecteurDirecteursTriangle(_mesh, vertID0, faceID0);
    VectorT <float,3> AB;
        AB[0] = vec[0];
        AB[1] = vec[1];
        AB[2] = vec[2];
    VectorT <float,3> AC;
        AC[0] = vec[3];
        AC[1] = vec[4];
        AC[2] = vec[5];

    // Calcul de la normal des deux vecteurs
    VectorT <float,3> N;
    N[0] = (AB[1]*AC[2] - AB[2] * AC[1]);
    N[1] = (AB[2]*AC[0] - AB[0] * AC[2]);
    N[2] = (AB[0]*AC[1] - AB[1] * AC[0]);


    // deuxième face
    VectorT <float,6> vec1 = VecteurDirecteursTriangle(_mesh, vertID1, faceID1);
    VectorT <float,3> AB1;
        AB1[0] = vec1[0];
        AB1[1] = vec1[1];
        AB1[2] = vec1[2];
    VectorT <float,3> AC1;
        AC1[0] = vec1[3];
        AC1[1] = vec1[4];
        AC1[2] = vec1[5];

    // Calcul de la normal des deux vecteurs
    VectorT <float,3> N1;
    N1[0] = (AB1[1]*AC1[2] - AB1[2] * AC1[1]);
    N1[1] = (AB1[2]*AC1[0] - AB1[0] * AC1[2]);
    N1[2] = (AB1[0]*AC1[1] - AB1[1] * AC1[0]);
/*
    if(faceID0 > faceID1)
        return -acos(N.normalized()|N1.normalized());
    else
        return acos(N.normalized()|N1.normalized());
*/

    if(pos == true)
        return -acos(N.normalized()|N1.normalized());
    else
        return acos(N.normalized()|N1.normalized());
}

VectorT <float,6> MainWindow::VecteurDirecteursTriangle(MyMesh *_mesh, int vertexID, int faceID){
    FaceHandle fh = _mesh->face_handle(faceID);
    std::vector <int> pointID;
    VectorT <float,3> points[3];
    int A = -1;
    int C = -1, B = -1;

     // On donne au point A son ID
    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(fh); curVert.is_valid(); curVert ++)
    {
       pointID.push_back((*curVert).idx());
       if(vertexID == (*curVert).idx())
           A = pointID.size()-1;
    }

    // affectation des points trouvés
    for (int i=0; i<pointID.size();i++)
    {
        VertexHandle vh = _mesh->vertex_handle(pointID.at(i));
        points[i][0] = _mesh->point(vh)[0];
        points[i][1] = _mesh->point(vh)[1];
        points[i][2] = _mesh->point(vh)[2];
    }

    // On donne aux points B et C leur ID
    for(int i = 0; i<3 ; i++){
        if(i != A){
            if(B == -1)
                B = i;
            else if(C == -1)
                C = i;
        }
    }

    // Calcul des vecteurs AB et AC
    VectorT <float,3> AB = points[B] - points[A];
    VectorT <float,3> AC = points[C] - points[A];
    VectorT <float,6> Vec;
    Vec[0] = AB[0]; Vec[1] = AB[1]; Vec[2] = AB[2];
    Vec[3] = AC[0]; Vec[4] = AC[1]; Vec[5] = AC[2];

    return Vec;
}



VectorT <float,3> MainWindow::LongueurArc(MyMesh *_mesh, int vertexID, int vertexID2){

    VectorT <float,3> points[2];

    // affectation des points trouvés
    for (int i=0; i<2;i++)
    {
        VertexHandle vh;

        if(i == 0)
            vh = _mesh->vertex_handle(vertexID);
        else if(i == 1)
            vh = _mesh->vertex_handle(vertexID2);

        points[i][0] = _mesh->point(vh)[0];
        points[i][1] = _mesh->point(vh)[1];
        points[i][2] = _mesh->point(vh)[2];
    }

    // Calcul des vecteurs AB et AC
    VectorT <float,3> AB = points[1] - points[0];

    return AB;
}



int MainWindow::PointEnFace(MyMesh *_mesh, int vertexID, int faceID, int faceID2){
    FaceHandle fh = _mesh->face_handle(faceID);
    FaceHandle fh1 = _mesh->face_handle(faceID2);

    int A = -1;
    int C = -1, B = -1;
    int A1 = -1;
    int C1 = -1, B1 = -1;

     // On donne au point A son ID
    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(fh); curVert.is_valid(); curVert ++)
    {
       if(vertexID == (*curVert).idx())
           A = (*curVert).idx();
       else if(B == -1)
           B = (*curVert).idx();
       else if(C == -1 && B != -1)
           C = (*curVert).idx();
    }

    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(fh1); curVert.is_valid(); curVert ++)
    {
        if(vertexID == (*curVert).idx())
            A1 = (*curVert).idx();
        else if(B1 == -1)
            B1 = (*curVert).idx();
        else if(C1 == -1 && B1 != -1)
            C1 = (*curVert).idx();
    }

    if(B == B1)
        return B;
    if(C == C1)
        return C;
    if(B == C1)
        return B;
    if(C == B1)
        return C;
    return A;
}

float MainWindow::angleEE(MyMesh* _mesh, int vertexID,  int faceID)
{
    VectorT <float,6> vec = VecteurDirecteursTriangle(_mesh, vertexID, faceID);
    VectorT <float,3> AB;
        AB[0] = vec[0];
        AB[1] = vec[1];
        AB[2] = vec[2];
    VectorT <float,3> AC;
        AC[0] = vec[3];
        AC[1] = vec[4];
        AC[2] = vec[5];

    AireBarycentrique(_mesh, vertexID);
    return acos(AB.normalized()|AC.normalized());
}

float MainWindow::fctH(MyMesh* _mesh, int vertexID){

    float firstPart = 1/(4.0*AireBarycentrique(_mesh, vertexID));
    float secondPart = 0;

    qDebug() << "\nvertex Actuel : " << vertexID;
    VertexHandle v_it = _mesh->vertex_handle(vertexID);
    // parcours des faces autour de vertexID

    std::vector<FaceHandle> Faces;
    for(MyMesh::VertexFaceIter  vf_it = mesh.vf_iter(v_it); vf_it; ++vf_it) {
        FaceHandle fh = *vf_it;
        Faces.push_back(fh);
        qDebug() << "id de la face : " << fh.idx();
    }

   for(int i =0; i<Faces.size()-1;i++) {
       FaceHandle fh = Faces.at(i);
       FaceHandle fh1 = Faces.at((i+1));

       if(fh.idx() != fh1.idx()){
           int vertexEnFace = PointEnFace(_mesh, vertexID, fh.idx(), fh1.idx());

           VectorT <float,3> AB = LongueurArc(_mesh, vertexID, vertexEnFace);

           qDebug() << "vertex courant :" << vertexID << "vertex en face : " << vertexEnFace;
           qDebug() << "face 1 : " << fh.idx() << "face 2 :" << fh1.idx();

           qDebug() << "** angleFF : " << angleFF(_mesh, fh.idx(), fh1.idx(), vertexID, vertexEnFace) << " norm de AB : " << AB.norm();
           secondPart += (angleFF(_mesh, fh.idx(), fh1.idx(), vertexID, vertexEnFace) * AB.norm());

       }
   }
    qDebug() << "firstPart :" << firstPart << " secondPart : " << secondPart << "\n";
    return firstPart * secondPart;
}

void MainWindow::H_Curv(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        VertexHandle vh = *curVert;
        float value = fctH(_mesh, vh.idx());
        _mesh->data(vh).value = value;
    }
}

float MainWindow::AireBarycentrique(MyMesh* _mesh, int vertexID){
    float aireTotal;
     VertexHandle v_it = _mesh->vertex_handle(vertexID);

     // parcours des faces autour de vertexID
    for(MyMesh::VertexFaceIter  vf_it = mesh.vf_iter(v_it); vf_it; ++vf_it) {
        FaceHandle fh = *vf_it;
        aireTotal += faceArea(_mesh, fh.idx());
    }
    return 1/3.0*aireTotal;
}

float MainWindow::AngleAbs(MyMesh* _mesh, int vertexID){
    float angleTotal;
     VertexHandle v_it = _mesh->vertex_handle(vertexID);

    // parcours des faces autour de vertexID
    for(MyMesh::VertexFaceIter  vf_it = mesh.vf_iter(v_it); vf_it; ++vf_it) {
        FaceHandle fh = *vf_it;
        angleTotal += angleEE(_mesh, v_it.idx(), fh.idx());
    }
    return angleTotal;
}

float MainWindow::fctK(MyMesh* _mesh, int vertexID){
    float firstPart = 1/AireBarycentrique(_mesh, vertexID);
    float secondPart = 2*M_PI - AngleAbs(_mesh, vertexID);
    return firstPart * secondPart;
}

void MainWindow::K_Curv(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        VertexHandle vh = *curVert;
        float value = fctK(_mesh, vh.idx());
        _mesh->data(vh).value = value;
    }
}
/* **** fin de la partie à compléter **** */



/* **** début de la partie boutons et IHM **** */
void MainWindow::on_pushButton_H_clicked()
{
    H_Curv(&mesh);
    displayMesh(&mesh, true); // true permet de passer en mode "carte de temperatures", avec une gestion automatique de la couleur (voir exemple)
}

void MainWindow::on_pushButton_K_clicked()
{
    K_Curv(&mesh);
    displayMesh(&mesh, true); // true permet de passer en mode "carte de temperatures", avec une gestion automatique de la couleur (voir exemple)
}

/*
    Cette fonction est à utiliser UNIQUEMENT avec le fichier testAngleArea.obj
    Elle est appelée par le bouton "Test angles/aires"

    Elle permet de vérifier les fonctions faceArea, angleFF et angleEE.
    Elle doit afficher :

    Aire de la face 0 : 2
    Aire de la face 1 : 2
    Angle entre les faces 0 et 1 : 1.5708
    Angle entre les faces 1 et 0 : -1.5708
    Angle au sommet 1 sur la face 0 : 0.785398
*/

void MainWindow::on_pushButton_angleArea_clicked()
{
    qDebug() << "Aire de la face 0 :" << faceArea(&mesh, 0);
    qDebug() << "Aire de la face 1 :" << faceArea(&mesh, 1);

    qDebug() << "Angle entre les faces 0 et 1 :" << angleFF(&mesh, 0, 1, 1, 2);
    qDebug() << "Angle entre les faces 1 et 0 :" << angleFF(&mesh, 1, 0, 1, 2);

    qDebug() << "Angle au sommet 1 sur la face 0 :" << angleEE(&mesh, 1, 0);
    qDebug() << "Angle au sommet 3 sur la face 1 :" << angleEE(&mesh, 3, 1);
}

void MainWindow::on_pushButton_chargement_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());

    mesh.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh);
}
/* **** fin de la partie boutons et IHM **** */

/* **** fonctions supplémentaires **** */
// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, bool isTemperatureMap, float mapRange)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(isTemperatureMap)
    {
        QVector<float> values;

        if(mapRange == -1)
        {
            for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
                values.append(fabs(_mesh->data(*curVert).value));
            qSort(values);
            mapRange = values.at(values.size()*0.8);
            qDebug() << "mapRange" << mapRange;
        }

        float range = mapRange;
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }
    else
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }


    ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    vertexSelection = -1;
    edgeSelection = -1;
    faceSelection = -1;

    modevoisinage = false;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
