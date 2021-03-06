///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synth�se d'images - Mod�lisation g�om�trique
// Auteur : Gilles Gesqui�re
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de cr�er des formes de bases.
// La forme repr�sent�e ici est un polygone blanc dessin� sur un fond rouge
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Vector.h"
#include "Point.h"
#include <iostream>"

using namespace std;

/* Dans les salles de TP, vous avez g�n�ralement acc�s aux glut dans C:\Dev. Si ce n'est pas le cas, t�l�chargez les .h .lib ...
Vous pouvez ensuite y faire r�f�rence en sp�cifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>.
Si vous mettez glut dans le r�pertoire courant, on aura alors #include "glut.h"
*/

#include <GL/freeglut.h>

// D�finition de la taille de la fen�tre
#define WIDTH  480
#define HEIGHT 480

// D�finition de la couleur de la fen�tre
#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1


// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27


//nbPoints du tableau



// Ent�tes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//affcihage points et vecteurs (tp1)
void dessinePoint (const Point& p, int taille);
void dessineVector (const Point& p, Vector& v);

//affichage courbe (tp2)
void drawCurve(const Point* tabPointsOfCurve, const long nbPoints);
void drawPoints(const Point* tab, const long nbPoints);

//calcul des coefficients  - HERMINET CURVE
double Fone(double positionU);
double Ftwo (double U);
double Fthree (double U);
double Ffor (double U);
Point* HermiteCubicCurve(Point p0, Point p1, Vector v0, Vector v1,long nbU);

//Bezier curve
double Fact(double val);
double Newton(int nbControlPoint, int CurrentControlPoint);
Point* BezierCurve(Point* tabControlePoint, long nbControlPoint, long nbU);
Point* getPosCasteljau(Point* tabControlPoint,long nbControlPoint, double u);
Point getPosCasteljau2(Point p1, Point p2, double u);
Point* BezierCurveByCasteljau(Point* tabControlPoint, long nbControlPoint, long nbU);

//surfaces

void drawSurfaceCylindric (Point* courbe, Vector v, int U, int V);
void dessineDroite(Point p1, Point p2);
void drawSurfaceReglee(Point* courbe1, Point* courbe2, int U, int V);
void drawSurfaceByCasteljau(Point** matrice, int nbPointControlU, int nbPointControlV, double u, double v);
Point** getMatrice(Point* tabControlPointU, long nbPointControlU,Point* tabControlPointV, long nbPointControlV);
void afficheMatrice(Point** matrice, int tailleU, int tailleV);

int main(int argc, char **argv)
{
  // initialisation  des param�tres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);

  // d�finition et cr�ation de la fen�tre graphique, ainsi que son titre
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Premier exemple : carr�");

  // initialisation de OpenGL et de la sc�ne
  initGL();
  init_scene();

  // choix des proc�dures de callback pour
  // le trac� graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fen�tre
  glutReshapeFunc(&window_reshape);
  // la gestion des �v�nements clavier
  glutKeyboardFunc(&window_key);

  // la boucle prinicipale de gestion des �v�nements utilisateur
  glutMainLoop();

  return 1;
}

// initialisation du fond de la fen�tre graphique : noir opaque
GLvoid initGL()
{
  glClearColor(RED, GREEN, BLUE, ALPHA);
}

// Initialisation de la scene. Peut servir � stocker des variables de votre programme
// � initialiser
void init_scene()
{
}

// fonction de call-back pour l�affichage dans la fen�tre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // C'est l'endroit o� l'on peut dessiner. On peut aussi faire appel
  // � une fonction (render_scene() ici) qui contient les informations
  // que l'on veut dessiner
  render_scene();

  // trace la sc�ne grapnique qui vient juste d'�tre d�finie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fen�tre

GLvoid window_reshape(GLsizei width, GLsizei height)
{
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  // possible de changer la taille de l'objet dans la fen�tre. Augmentez ces valeurs si l'objet est
  // de trop grosse taille par rapport � la fen�tre.
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

  // toutes les transformations suivantes s�appliquent au mod�le de vue
  glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des �v�nements clavier

GLvoid window_key(unsigned char key, int x, int y)
{
  switch (key) {
  case KEY_ESC:
    exit(1);
    break;

  default:
    printf ("La touche %d n�est pas active.\n", key);
    break;
  }
}



//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{
//D�finition de la couleur
 glColor3f(1.0, 1.0, 1.0);

  //  Nous cr�ons ici un polygone. Nous pourrions aussi cr�er un triangle ou des lignes. Voir ci-dessous les parties
  // en commentaires (il faut commenter le bloc qui ne vous int�resse pas et d�commenter celui que vous voulez tester.

   // Cr�ation de deux lignes
	/*glBegin(GL_LINES);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();*/

 // cr�ation d'un polygone
/*	glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
*/

// cr�ation d'un triangle
/*	glBegin(GL_TRIANGLES);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
	glEnd();
*/

//=================affichage point et vecteurs - tp1 =============

/*Point p1 = Point(0, 1, 0);//point � projeter
Point p2 = Point(1, 0, 0);//fin du vecteur
Point p0 = Point(-1, 0, 0);//d�but du vecteur
Vector v1 = Vector(2, 0, 0.0);//vecteur sur lequel on projette
//Point p3 = p1.ProjectOnLine(p0, p2);
Point p3 = p1.ProjectOnLine(v1,p0);
Vector v2 = Vector (p3.getX()-p1.getX(), p3.getY()-p1.getY(),p3.getZ()-p1.getZ());

dessinePoint (p0);
dessinePoint (p1);
dessinePoint (p2);
dessineVector(p0,  v1);
dessinePoint (p3);
dessineVector(p1, v2);
*/

//========================affichage de courbes - tp2 =============

Point* tabPointsOfCurve = new Point[4];
tabPointsOfCurve[0] = Point(-1, -1, 0);
tabPointsOfCurve[1] = Point(-0.5, -0.5, 0);
tabPointsOfCurve[2] = Point(0, -0.5, 0);
tabPointsOfCurve[3] = Point(0.5, -1, 0);
//drawCurve(tabPointsOfCurve, 4);
//Point p0 = Point(0,0,0);
Point p1 = Point(2,0,0);
Vector v0 = Vector (1,1,0);
Vector v1 = Vector (1,-1,0);
long nbU = 9;

//drawCurve(HermiteCubicCurve(p0,p1,v0,v1,nbU),nbU);
//glColor3f(1.0, 1.0, 0);
//drawCurve(BezierCurve(tabPointsOfCurve,4,nbU),nbU);

//drawPoints(BezierCurveByCasteljau(tabPointsOfCurve,4,20),20);

//drawCurve(BezierCurveByCasteljau(tabPointsOfCurve,4,nbU),nbU);

//====================affichage de surfaces - TP3 ======================

Point* tab = new Point [6];
//Point p0 = Point (-1,0,0);
//dessinePoint(p0);
//Vector v0 = Vector (0,-1,0);
//Point p = Point(-1,-0.5,0);
//dessinePoint(p);
//dessineVector (p0, v0);
//dessineDroite(p0,p1);

tab [0] = Point (-1.7,1,0);
tab [1] = Point (-0.1,1.7,0);
tab [2] = Point (0.5,1,0);
tab [3] = Point (1.2,1.6,0);
tab [4] = Point (1.7,0.8,0);
tab [5] = Point (2,1.5,0);
/*tab [6] = Point (0.2,0.8,0);
tab [7] = Point (0.4,0.6,0);
tab [8] = Point (0.6,0.4,0);
tab [9] = Point (0.8,0.2,0);*/
//drawCurve(tab,6);
glColor3f(1.0, 0, 1.0);


//glBegin(GL_LINES);
//Point* poi = BezierCurveByCasteljau(tab,6,20);
//glEnd();


//drawPoints(poi,20);// =====>  Pour le TP2
//drawCurve(BezierCurve(tabPointsOfCurve,6,nbU),nbU);

Point* tab1 = new Point [4];
tab1 [0] = Point (0,1,0);
tab1 [1] = Point (0,0.7,0);
tab1 [2] = Point (0,0.3,0);
tab1 [3] = Point (0,0,0);

glColor3f(1.0, 1.0, 0);//jaune
drawCurve(tab,6);

glColor3f(1.0, 0, 1.0);//rose
drawCurve(tab1,4);


//drawSurfaceCylindric(BezierCurve(tab,4,10),v0,10,4);
//drawSurfaceReglee(BezierCurve(tab,6,50),BezierCurve(tab1,2,50),50,20);

/*Point p8 = Point(0,0,0);
Point p9 = Point (1,1,1);
Vector v =p8.createVector(p9);
dessineVector(p8,v);
Point p12 = getPosCasteljau2(p8,p9,0.5);
dessinePoint(p12,3);*/

Point** mat = getMatrice(tab,6,tab1,4);
afficheMatrice(mat,6,4);
cout<<"draw"<<endl;
drawSurfaceByCasteljau(mat,6,4,20,20);
}

//=============================================================

void afficheMatrice(Point** matrice, int tailleU, int tailleV)
{
    glPointSize(5);
    glBegin(GL_POINTS);

    for(int i =0; i<tailleU;i++)
    {
        for(int j =0;j<tailleV;j++)
        {
            glVertex3f(matrice[i][j].getX(),matrice[i][j].getY(),matrice[i][j].getZ());
        }
    }
     glEnd();
}
//======================functions==============================
//=============================================================

//====================fonctions tp1============================
void dessinePoint(const Point& p, int taille)
{
   glPointSize(taille);
    glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_POINTS);
   glVertex3f(p.getX(),p.getY(),p.getZ());
   glEnd();
}


void dessineVector(const Point& p, Vector& v)
{
    glBegin(GL_LINES);
    glVertex3f(p.getX(), p.getY(), p.getZ());
    glVertex3f(p.getX()+v.getX(), p.getY()+v.getY(), p.getZ()+v.getZ());
    glEnd();
}

//===================fonctions tp2==============================
void drawCurve(const Point* tab, const long nbPoints)
{
    glBegin(GL_LINE_STRIP);
    for (int i =0 ; i<nbPoints ; i++)
    {
       glVertex3f(tab[i].getX(),tab[i].getY(),tab[i].getZ());
    }
    glEnd();

    for (int i =0; i <nbPoints; i++)
    {
       dessinePoint(tab[i],1);
    }
}

void drawPoints(const Point* tab, const long nbPoints)
{
    for (int i =0; i <nbPoints; i++)
    {
       dessinePoint(tab[i],3);
    }
}


double Fone(double u)
{
  return (2*(pow(u,3))-3*(pow(u,2))+1);
}
double Ftwo (double u)
{
    return (-2*(pow(u,3))+3*(pow(u,2)));
}
double Fthree (double u)
{
    return (pow(u,3)- 2*(pow(u,2)) + u);
}
double Ffor (double u)
{
    return (pow(u,3)- pow(u,2));
}

Point* HermiteCubicCurve(Point p0, Point p1, Vector v0, Vector v1, long nbU)
{

    Point* res = new Point [nbU];
    for(int i=1;i<=nbU;i++)
    {
      double u = (double) i/nbU;
      double X = Fone(u)*p0.getX()+Ftwo(u)*p1.getX()+Fthree(u)*v0.getX()+Ffor(u)*v1.getX();
      double Y = Fone(u)*p0.getY()+Ftwo(u)*p1.getY()+Fthree(u)*v0.getY()+Ffor(u)*v1.getY();
      double Z = Fone(u)*p0.getZ()+Ftwo(u)*p1.getZ()+Fthree(u)*v0.getZ()+Ffor(u)*v1.getZ();
      Point p = Point( X, Y, Z);
      res[i] = p;
    }

return res;
}

double Fact(double n) {
	if (n == 0) {
		return 1.0;
	}
	return n*Fact(n-1);
}


double Newton(long nbControlPoint, int CurrentControlPoint, double u)
{
    int n = nbControlPoint;
    int i = CurrentControlPoint;
   double b = (Fact(n)/(Fact(i)*Fact(n-i)))*pow(u,i)*pow((1-u),(n-i));
   return b;
}

Point* BezierCurve(Point* tabControlPoint, long nbControlPoint, long nbU)
{

	double n = nbControlPoint-1;
	Point* pts = new Point[nbU];

	for (int j = 0; j < nbU; ++j) {
		double u = 1.0/(nbU-1) * (double) j;
		Point p = Point();
		for (int i = 0; i <= n; ++i) {
			double Bni = (Fact(n) / (Fact(i) * Fact(n-i))) * pow(u, i) * pow(1-u, n-i);
			p.setX(p.getX() + Bni*tabControlPoint[i].getX());
			p.setY(p.getY() + Bni*tabControlPoint[i].getY());
			p.setZ(p.getZ() + Bni*tabControlPoint[i].getZ());
		}
		pts[j] = p;
	}
	return pts;
}



Point getPosCasteljau2(Point p1, Point p2, double u)
{

    Vector v =p1.createVector(p2);
  //  v.afficher();
    Point p = Point ();
   // v.Normalized();
    p= p1.multiply(v,u);

    if(u == 0.3)
    {
dessineDroite(p1,p2);
    }
    return p ;
}

Point* getPosCasteljau(Point* tabControlPoint,long nbPtCont, double u)
{
    //s'il n'ya qu'un seul point de controle le piont final prendra sa valeur
//Point* tab1 = new Point [1];
 Point* res = new Point[(int)nbPtCont-1];
if (nbPtCont == 1)
{

    return tabControlPoint;
   //  return tab1;
}


else
{

   for (int i =0;i<nbPtCont-1;i++)
   {
       Point p1 = tabControlPoint[i];
       Point p2 = tabControlPoint[i+1];
       Point p12 = getPosCasteljau2(p1,p2,u);
        res [i] = p12;

        if(u == 0.3)
        {



       // glVertex3f(p12.getX(),p12.getY(),p12.getZ());
        }
}

   return getPosCasteljau(res,nbPtCont-1,u);
}
}

Point* BezierCurveByCasteljau(Point* tabControlPoint, long nbControlPoint, long nbU)
{
    //int n = nbControlPoint-1;
    //cr�ation du tableau qui contiendra les nouveaux points.
    Point* res = new Point[nbU];
    for (int i=1;i<=nbU;i++)
    {
        double u = (double)i/nbU;
        Point* tab = getPosCasteljau(tabControlPoint,nbControlPoint,u);
        res[i] = tab[0];
    }nbPointControlU
return res;
}


//======================fonctions TP3 =========================
//==============================================================

void dessineDroite(Point p1, Point p2)
{
   glBegin(GL_LINES);
    glVertex3f(p1.getX(), p1.getY(), p1.getZ());
    glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    glEnd();
}



void drawSurfaceCylindric(Point* courbe, Vector vec, int U, int V)
{

	Point* res = new Point [U*V];
    for (int i = 0; i<U; i++)
    {
        Point pu = courbe [i];
        Point qu = pu.addit(vec);
        for( int j = 0; j < V; j ++)
        {
            double v = (double)j/V;
            double X = (1-v) * pu.getX() + v * qu.getX();
            double Y = (1-v) * pu.getY() + v * qu.getY();
            double Z = (1-v) * pu.getZ() + v * qu.getZ();
      Point p = Point( X, Y, Z);
      res[j] = p;
        }
        drawPoints(res,4);
    }
}

void drawSurfaceReglee(Point* courbe1, Point* courbe2, int U, int V)
{
    Point* res = new Point [U*V];
    for (int i = 0; i<U; i++)
    {
        double u = (double)(i/U-1);
        Point pu = courbe1[i];
        Point qu = courbe2[i];

        for (int j = 0; j<V;j++)
        {
            double v = (double)j/V;
            double X = (1-v) * pu.getX() + v * qu.getX();
            double Y = (1-v) * pu.getY() + v * qu.getY();
            double Z = (1-v) * pu.getZ() + v * qu.getZ();
      Point p = Point( X, Y, Z);
      res[j] = p;
        }
         drawPoints(res,V);
    }
}

Point** getMatrice(Point* tabControlPointU, long nbPointControlU,Point* tabControlPointV, long nbPointControlV)
{

    Point** matrice = new Point* [nbPointControlU];
    for(int k =0;k<nbPointControlU;k++)
    {
        matrice[k] = new Point [nbPointControlV];
    }


    for(int i =0; i < nbPointControlU;i++)
    {
        Point pU = tabControlPointU[i];
        Point pV = tabControlPointV[0];nbPointControlU

        Vector trans = pV.createVector(pU);

        for( int j =0; j<nbPointControlV;j++)
        {
           matrice[i][j] = tabControlPointV[j].addit(trans);
        }
    }
return matrice;
}

Point** getMatricePlusPetite(Point** matrice, double u, double v, int nbU, int nbV)
{
    cout << "nbU=" << nbU << endl;
    cout<<"fonctionprincipal"<<endl;
    if(nbU==1 && nbV==1)
    {
        cout<<"boucleinfinie"<<endl;
        return matrice;
    }

    Point** matricePetite = new Point* [nbU-1];

    for(int n =0;n<nbU-1;n++)
    {

        matricePetite[n] = new Point[nbV-1];

    }
cout<<"matriceOk"<<endl;
    for(int i =0;i<nbU-1;i++)
    {
        for(int j = 0;j< nbV-1;j++)
        {
          //  cout<<"j="<<j<<endl;
          //  cout<<"i="<<i<<endl;
            Point pij = matrice[i][j];
          //  cout << "calcul piPlus..." << endl;
            Point piPlus = matrice [i+1][j];
          //  cout << "calcul pijplus..." << endl;
            Point pijPlus = matrice [i+1][j+1];
          //  cout << "calcul pjPlus..." << endl;nbPointControlU
            Point pjPlus = matrice [i][j+1];

           // cout << "calcul vectors..." << endl;

            Vector A = pij.createVector(piPlus);
            Vector B = pjPlus.createVector(pijPlus);

            Point a = pij.multiply(A,u);
            Point b = pjPlus.multiply(B,u);

            Vector AB = a.createVector(b);

            Point m = a.multiply(AB,v);

            matricePetite[i][j] = m;
        }
    }
    return getMatricePlusPetite(matricePetite,u,v,nbU-1,nbV-1);
}

void drawSurfaceByCasteljau(Point** matrice, int nbPointControlU, int nbPointControlV, double nbu, double nbv)
{
cout<<"drawsurface"<<endl;
    for(int i= 0; i<nbu;i++)
    {
        for(int j =0;j<nbv;j++)
        {
            cout<<"deuxi�me for"<<endl;
            double u = (double)(i/nbu-1);
            double v = (double)(j/nbv-1);
            matrice = getMatricePlusPetite(matrice, u,v,nbu,nbv);
            cout<<"hello"<<endl;
            matrice[0][0].afficher();
            dessinePoint(matrice[0][0],1);



        }
    }
}
