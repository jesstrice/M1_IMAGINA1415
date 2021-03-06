/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.cpp
*
* Description : Voir le fichier .h
*
* Auteur : Mickael Pinto
*
* Mail : mickael.pinto@live.fr
*
* Date : Octobre 2012
*
*******************************************************************************/

#include "ImageBase.h"
#include "image_ppm.h"
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf




ImageBase::ImageBase(void)
{
	isValid = false;
	init();
}

ImageBase::ImageBase(int imWidth, int imHeight, bool isColor)
{
	isValid = false;
	init();

	color = isColor;
	height = imHeight;
	width = imWidth;
	nTaille = height * width * (color ? 3 : 1);

	if(nTaille == 0)
		return;

	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;
}


ImageBase::~ImageBase(void)
{
	reset();
}

void ImageBase::init()
{
	if(isValid)
	{
		free(data);
		free(dataD);
	}

	data = 0;
	dataD = 0;
	height = width = nTaille = 0;
	isValid = false;
}

void ImageBase::reset()
{
	if(isValid)
	{
		free(data);
		free(dataD);
	}
	isValid = false;
}

void ImageBase::load(char *filename)
{
	init();

	int l = strlen(filename);

	if(l <= 4) // Le fichier ne peut pas etre que ".pgm" ou ".ppm"
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que '.pgm' ou '.ppm'");
		exit(0);
	}

	int nbPixel = 0;

	if( strcmp(filename + l - 3, "pgm") == 0) // L'image est en niveau de gris
	{
		color = false;
		lire_nb_lignes_colonnes_image_pgm(filename, &height, &width);
		nbPixel = height * width;

		nTaille = nbPixel;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_pgm(filename, data, nbPixel);
	}
	else if( strcmp(filename + l - 3, "ppm") == 0) // L'image est en couleur
	{
		color = true;
		lire_nb_lignes_colonnes_image_ppm(filename, &height, &width);
		nbPixel = height * width;

		nTaille = nbPixel * 3;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_ppm(filename, data, nbPixel);
	}
	else
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que .pgm ou .ppm");
		exit(0);
	}

	dataD = (double*)malloc(sizeof(double) * nTaille);

	isValid = true;
}

bool ImageBase::save(char *filename)
{
	if(!isValid)
	{
		printf("Sauvegarde de l'image impossible : L'image courante n'est pas valide");
		exit(0);
	}

	if(color)
		ecrire_image_ppm(filename, data,  height, width);
	else
		ecrire_image_pgm(filename, data,  height, width);

	return true;
}

ImageBase *ImageBase::getPlan(PLAN plan)
{
	if( !isValid || !color )
		return 0;

	ImageBase *greyIm = new ImageBase(width, height, false);

	switch(plan)
	{
	case PLAN_R:
		planR(greyIm->data, data, height * width);
		break;
	case PLAN_G:
		planV(greyIm->data, data, height * width);
		break;
	case PLAN_B:
		planB(greyIm->data, data, height * width);
		break;
	default:
		printf("Il n'y a que 3 plans, les valeurs possibles ne sont donc que 'PLAN_R', 'PLAN_G', et 'PLAN_B'");
		exit(0);
		break;
	}

	return greyIm;
}

void ImageBase::copy(const ImageBase &copy)
{
	reset();

	isValid = false;
	init();

	color = copy.color;
	height = copy.height;
	width = copy.width;
	nTaille = copy.nTaille;
	isValid = copy.isValid;

	if(nTaille == 0)
		return;

	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;

	for(int i = 0; i < nTaille; ++i)
	{
		data[i] = copy.data[i];
		dataD[i] = copy.dataD[i];
	}

}

unsigned char *ImageBase::operator[](int l)
{
	if(!isValid)
	{
		printf("L'image courante n'est pas valide");
		exit(0);
	}

	if((!color && l >= height) || (color && l >= height*3))
	{
		printf("L'indice se trouve en dehors des limites de l'image");
		exit(0);
	}

	return data+l*width;
}

//==========================fonstionc tp1=========================
//==========================affichage=============================
void ImageBase::affichePix (bool ligne, int indice_ligne)
{
  std::filebuf fb;
  fb.open ("profil.txt",std::ios::out);
  std::ostream os(&fb);

    if(ligne)
    {
       for (int i = (width*indice_ligne);i <= (width*indice_ligne)+width; i++)
          {
           os << i <<" "<< (int)data[i]<<"\n";
          }
    }
  fb.close();
}

//===============================histogramme======================
void ImageBase::histo()
{
    std::filebuf fb;
  fb.open ("histo.dat",std::ios::out);
  std::ostream os(&fb);

       for (int i = 0;i <= 255; i++)
          {
              int nb_occ =0;
              for (int j = 0; j <= width*height; j++)
              {
                if(data[j]==i)
                 nb_occ++;
              }
           os << i <<" "<< nb_occ<<"\n";
          }

  fb.close();
}

//========================fonctions TP2=============================
//========================�rosion===================================
void ImageBase::erosion ()
{
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    for (int i = 2 ;i< width - 1; i++)
    {
        for (int j = 2; j< height - 1; j++)
        {
            if()
            {
                imOut[i+1][j] = 255;
                imOut[i-1][j] = 255;
                imOut[i][j+1] = 255;
                imOut[i][j-1] = 255;
                imOut[i+1][j+1] = 255;
                imOut[i-1][j+1] = 255;
                imOut[i+1][j-1] = 255;
                imOut[i-1][j-1] = 255;

            }
        }
    }
    return imOut;
   // imOut.save("imErodee.pgm");
}

ImageBase ImageBase::inversion(ImageBase imIn)
{
 ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
     for (int i = 0 ;i< imIn.width ; i++)
    {
        for (int j = 0; j< imIn.height ; j++)
        {
            if (imIn[i][j]==0)
                imOut [i][j]= 255;
            else
                imOut[i][j]==0;
        }
    }
    return imOut;  // imOut.save("imInverse.pgm");
}
ImageBase ImageBase::dilatation (ImageBase imIn)
{
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
 // ImageBase imOut2(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
//imOut2=imOut.inversion(imOut);
//   imOut.erosion(imOut);
 //  imOut.inversion(imOut);

   for (int i = 2 ;i< imIn.width - 1; i++)
    {
        for (int j = 2; j< imIn.height - 1; j++)
        {
            if( imIn[i+1][j]==0 || imIn[i-1][j]==0 || imIn[i][j+1]==0 || imIn[i][j-1]==0)
                imOut[i][j]==80;
        }
    }
//imOut.save("imdilatee.pgm");
return imOut;
    //imOut.save("imDilatee.pgm");
}
