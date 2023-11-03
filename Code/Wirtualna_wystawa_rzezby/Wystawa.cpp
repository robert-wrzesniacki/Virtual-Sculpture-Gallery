
//Deklaracja bibliotek
#include <windows.h>		
#include <math.h>			
#include <stdio.h>			
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>	
#include <gl\glut.h>	


//Deklaracja zmiennych
HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool	keys[256];			// Tablica do klawiatury
bool	active=TRUE;		// Aktywny windows
bool	fullscreen=TRUE;	// Tryb pełnoekranowy
bool	blend;				// Blending ON/OFF
bool	bp;					// B wciśnięty?

bool  ent;
bool	enter;					// wciśnięty enter?
bool enter2;
bool wylacz = FALSE;
POINT mpos;

float jasno = 0.58;

const float piover180 = 0.0174532925f;
GLfloat X, Z;
float lastX, lastZ;
GLfloat HeadMovement, HeadMovAngle;
GLfloat Heading;
GLfloat Tilt;
int MouseSpeed = 7;
GLfloat MoveSpeed = 0.03;

//kolizje
float odleglosc1;
float odleglosc2;
float odleglosc3;
float posx1 =0.0;
float posx2 =3.7;
float posx3 =-3.5;
float posz = 4.25;
//info
float odl1;
float odl2;
float odl3;
float odldoor;
float odlrif1;
float odlrif2;
float odlrif3;
float odlrif4;
float posdoorx=0.0;
float posdoorz=-7.0;
float posrif1x=7.0;
float posrif1z=-2.5;
float posrif2x=-7.0;
float posrif2z=-3.0;
float posrif3x=7.0;
float posrif3z=2.7;
float posrif4x=-7.0;
float posrif4z=2.0;

char *napis = "";
//napis = "asddasdsada";


//zmienne tekstur
GLuint	podloga;			// tekstura pogłogi
GLuint	sufit;				// -||- sufitu
GLuint	sciana;		// -||- ściany
GLuint	pic1;			// -||- obraz1
GLuint	pic2;			// -||- 2
GLuint	pic3;		// -||- 3
GLuint	pic4;			// -||- 4
GLuint	pic5;			// -||- 5
GLuint	drzwi;				// -||- drzwi
GLuint	prog;				// -||- próg
GLuint	marmur;				// -||- marmur
GLuint info1;
GLuint info2;
GLuint info3;
GLuint info4;
GLuint info5;
GLuint info6;
GLuint info7;
GLuint textura;

//zmienna do czcionek
GLuint	base;
//zmienne modeli
GLuint model1 = -1;

extern GLint Gen3DObjectList();



LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Deklaracja dla WndProc

//Zamykanie okna


GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(200);								// Storage For 96 Characters

	font = CreateFont(	-15,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						EASTEUROPE_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Script");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 200, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 200);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

//Loader textur z .bmp
AUX_RGBImageRec *LoadBMP(char *Filename)                // Ładowanie bitmapy
{
        FILE *File=NULL;                                

        if (!Filename)                                  // sprawdza czy podano nazwę pliku
        {
                return NULL;                            // jeśli nie, to nic nie robi
        }

        File=fopen(Filename,"r");                       // otwiera plik

        if (File)                                       // Sprawdza czy istnieje
        {
                fclose(File);                           
                return auxDIBImageLoad(Filename);       // Ładuje mapę
        }
        return NULL;                                    // Jeśli się nie udało, to daje sobie z tym spokój
}

//Ładowanie tekstur
int LoadTextur()                                    
{
        int Status=FALSE;                               // deklaracja statusu

        AUX_RGBImageRec *TextureImage[20];               // tworzy tablicê dla tekstury

        memset(TextureImage,0,sizeof(void *)*1);        

// ³adowanie plików i sprawdzanie b³êdów, itp.
if (TextureImage[0]=LoadBMP("Data/podloga.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &podloga);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, podloga);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        }
if (TextureImage[1]=LoadBMP("Data/sufit.bmp"))
        {
                Status=TRUE;                           

                glGenTextures(1, &sufit);        

				glBindTexture(GL_TEXTURE_2D, sufit);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);

        }
if (TextureImage[2]=LoadBMP("Data/sciana.bmp"))
        {
                Status=TRUE;                   

                glGenTextures(1, &sciana);

				glBindTexture(GL_TEXTURE_2D, sciana);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);

        }
if (TextureImage[3]=LoadBMP("Data/pic1.bmp"))
        {
                Status=TRUE;

                glGenTextures(1, &pic1); 

				glBindTexture(GL_TEXTURE_2D, pic1);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);

        }
if (TextureImage[4]=LoadBMP("Data/pic2.bmp"))
        {
                Status=TRUE;

                glGenTextures(1, &pic2);

				glBindTexture(GL_TEXTURE_2D, pic2);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);

        }
if (TextureImage[5]=LoadBMP("Data/pic3.bmp"))
        {
                Status=TRUE; 

                glGenTextures(1, &pic3);  

				glBindTexture(GL_TEXTURE_2D, pic3);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[5]->sizeX, TextureImage[5]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[5]->data);

        }
        
if (TextureImage[6]=LoadBMP("Data/pic4.bmp"))
        {
                Status=TRUE;                           

                glGenTextures(1, &pic4);        

				glBindTexture(GL_TEXTURE_2D, pic4);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[6]->sizeX, TextureImage[6]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[6]->data);

        }
if (TextureImage[9]=LoadBMP("Data/pic5.bmp"))
        {
                Status=TRUE;                           

                glGenTextures(1, &pic5);        

				glBindTexture(GL_TEXTURE_2D, pic5);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[9]->sizeX, TextureImage[9]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[9]->data);

        }
if (TextureImage[7]=LoadBMP("Data/drzwi4.bmp"))
        {
                Status=TRUE;

                glGenTextures(1, &drzwi); 

				glBindTexture(GL_TEXTURE_2D, drzwi);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[7]->sizeX, TextureImage[7]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[7]->data);

        }
if (TextureImage[8]=LoadBMP("Data/prog.bmp"))
        {
                Status=TRUE;                   

                glGenTextures(1, &prog);

				glBindTexture(GL_TEXTURE_2D, prog);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[8]->sizeX, TextureImage[8]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[8]->data);

        }
if (TextureImage[10]=LoadBMP("Data/marmur.bmp"))
        {
                Status=TRUE;                   

                glGenTextures(1, &marmur);

				glBindTexture(GL_TEXTURE_2D, marmur);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[10]->sizeX, TextureImage[10]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[10]->data);

        }
if (TextureImage[11]=LoadBMP("Data/info1.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info1);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info1);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[11]->sizeX, TextureImage[11]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[11]->data);

        }
if (TextureImage[12]=LoadBMP("Data/info2.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info2);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info2);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[12]->sizeX, TextureImage[12]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[12]->data);

        }
if (TextureImage[13]=LoadBMP("Data/info3.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info3);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info3);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[13]->sizeX, TextureImage[13]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[13]->data);

        }
if (TextureImage[14]=LoadBMP("Data/info4.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info4);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info4);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[14]->sizeX, TextureImage[14]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[14]->data);

        }
if (TextureImage[15]=LoadBMP("Data/info5.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info5);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info5);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[15]->sizeX, TextureImage[15]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[15]->data);

        }
if (TextureImage[16]=LoadBMP("Data/info6.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info6);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info6);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[16]->sizeX, TextureImage[16]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[16]->data);

        }
if (TextureImage[17]=LoadBMP("Data/info7.bmp"))
        {
                Status=TRUE;                            

                glGenTextures(1, &info7);          // tworzy teksture

				// Parametry tekstury
				glBindTexture(GL_TEXTURE_2D, info7);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[17]->sizeX, TextureImage[17]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[17]->data);

        }

for (int loop=0; loop<17; loop++)                 // Loop Through 5 Textures
        {
            if (TextureImage[loop])                 // If Texture Exists
            {
                if (TextureImage[loop]->data)            // If Texture Image Exists
                {
                    free(TextureImage[loop]->data);      // Free The Texture Image Memory
                }
                free(TextureImage[loop]);           // Free The Image Structure
            }
        }
        return Status;                                  
}


//Przeskalowywanie okna
GLvoid Skalowanie(GLsizei width, GLsizei height)		
{
	if (height==0)										// czy wysokość = zero
	{
		height=1;										// jeśli tak, to ustawia ją na 1
	}

	glViewport(0,0,width,height);						// resetuje obecną pozycję okna

	glMatrixMode(GL_PROJECTION);						// wybiera macierz projekcji
	glLoadIdentity();									// resetuje ją

	// oblicza ratio
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// wybiera macierz widoku modelu 
	glLoadIdentity();									// resetuje ją
}

//Ustawienia
int Ustawienia(GLvoid)										
{
	if (!LoadTextur())								// Najpier ładuje sobie tekstury do zmiennych
	{
		return FALSE;									// jak się nie da to lipa i error
	}
	
 	glEnable(GL_TEXTURE_2D);							// Tekstury dwówymiarowe on
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Ustawienia Blendingu
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Kolor tła
	glClearDepth(1.0f);									// Czyści bufor głębi
	glDepthFunc(GL_LESS);								// test głębi
	glEnable(GL_DEPTH_TEST);							// Testowanie głębi on
	glShadeModel(GL_SMOOTH);							// wygładzenie on
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// perspektywa
	glEnable(GL_NORMALIZE);
	//Pozycja startowa
	Heading = 0;
	X = 0;
	Z = -3;

	//tworzenie czcionki
	BuildFont();

	//ładowanie modli do zmiennych
	model1 = Gen3DObjectList();

	return TRUE;										// wszystko się udało
}

void swiatlo(){
GLfloat LightAmbient[]=		{ jasno, jasno, jasno, 1.0f };
GLfloat LightDiffuse[]=		{ jasno, jasno, jasno, 1.0f };
GLfloat LightPosition[]=	{ 3.0f, 3.0f, 3.0f, 1.0f };
GLfloat LightPosition2[]=	{ -3.0f, 3.0f, -3.0f, 1.0f };
GLfloat LightPosition3[]=	{ -3.0f, 3.0f, 3.0f, 1.0f };
GLfloat LightPosition4[]=	{ 3.0f, 3.0f, -3.0f, 1.0f };
	glEnable( GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);	// Position The Light
	glEnable(GL_LIGHT2);								// Enable Light One

	glLightfv(GL_LIGHT3, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT3, GL_POSITION,LightPosition3);	// Position The Light
	glEnable(GL_LIGHT3);								// Enable Light One
	glLightfv(GL_LIGHT4, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT4, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT4, GL_POSITION,LightPosition4);	// Position The Light
	glEnable(GL_LIGHT4);	
}
void info(){
odldoor=sqrt((posdoorx-X)*(posdoorx-X)+(posdoorz-Z)*(posdoorz-Z));
odlrif1=sqrt((posrif1x-X)*(posrif1x-X)+(posrif1z-Z)*(posrif1z-Z));
odlrif2=sqrt((posrif2x-X)*(posrif2x-X)+(posrif2z-Z)*(posrif2z-Z));
odlrif3=sqrt((posrif3x-X)*(posrif3x-X)+(posrif3z-Z)*(posrif3z-Z));
odlrif4=sqrt((posrif4x-X)*(posrif4x-X)+(posrif4z-Z)*(posrif4z-Z));
odl1 = sqrt((posx1-X)*(posx1-X)+(posz-Z)*(posz-Z));
odl2 = sqrt((posx2-X)*(posx2-X)+(posz-Z)*(posz-Z));
odl3 = sqrt((posx3-X)*(posx3-X)+(posz-Z)*(posz-Z));
if (odldoor<=2){
napis = "Naciśnij klawisz [ENTER], by zakończyć działanie programu.";
}else if (odlrif1<=2.5){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info1; 
}else if (odlrif2<=2.5){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info2;
}else if (odlrif3<=2.5){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info3;
}else if (odlrif4<=2.5){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info4;
}else if (odl1<=1.7){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info5;
}else if (odl2<=1.7){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info6;
}else if (odl3<=1.7){
napis = "Naciśnij klawisz [ENTER], by zobaczyć informacje o obiekcie.";
textura =info7;
}else{napis="";
textura = 0;}
}
void kolizje(){
	
	if (-X<-6){
	X = 6;
	}
	if (-X>6){
	X = -6;
	}
	if (-Z<-6){
	Z = 6;
	}
	if (-Z>6){
	Z = -6;
	}

	odleglosc1 = sqrt((posx1-X)*(posx1-X)+(posz-Z)*(posz-Z));
	if (odleglosc1<=1.25){
	X = lastX;
	Z = lastZ;
	}
	odleglosc2 = sqrt((posx2-X)*(posx2-X)+(posz-Z)*(posz-Z));
	if (odleglosc2<=1.25){
	X = lastX;
	Z = lastZ;
	}
	odleglosc3 = sqrt((posx3-X)*(posx3-X)+(posz-Z)*(posz-Z));
	if (odleglosc3<=1.25){
	X = lastX;
	Z = lastZ;
	}

}
void obrazy(){
	glBindTexture(GL_TEXTURE_2D, pic3);
		glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-6.99f,1.75f,4.85f);
			glTexCoord2f(1.0f,0.0f); glVertex3f(-6.99f,1.75f,1.0f);
			glTexCoord2f(1.0f,1.0f); glVertex3f(-6.99f,3.7f,1.0f);	
		    glTexCoord2f(0.0f,1.0f); glVertex3f(-6.99f,3.7f,4.85f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, pic1);
		glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-6.99f,1.5f,-5.0f);
			glTexCoord2f(1.0f,0.0f); glVertex3f(-6.99f,1.5f,-1.0f);
			glTexCoord2f(1.0f,1.0f); glVertex3f(-6.99f,4.0f,-1.0f);	
		    glTexCoord2f(0.0f,1.0f); glVertex3f(-6.99f,4.0f,-5.0f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, pic2);
		glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(6.99f,1.5f,-4.0f);
			glTexCoord2f(1.0f,0.0f); glVertex3f(6.99f,1.5f,-1.0f);
			glTexCoord2f(1.0f,1.0f); glVertex3f(6.99f,3.5f,-1.0f);	
		    glTexCoord2f(0.0f,1.0f); glVertex3f(6.99f,3.5f,-4.0f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, pic5);
		glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(6.99f,1.5f,4.0f);
			glTexCoord2f(1.0f,0.0f); glVertex3f(6.99f,1.5f,1.0f);
			glTexCoord2f(1.0f,1.0f); glVertex3f(6.99f,3.5f,1.0f);	
		    glTexCoord2f(0.0f,1.0f); glVertex3f(6.99f,3.5f,4.0f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, drzwi);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(1.0f,1.0f); glVertex3f(-1.25f,4.0f,7.5f);
			glTexCoord2f(0.0f,1.0f); glVertex3f(1.25f,4.0f,7.5f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(1.25f,0.0f,7.5f);	
		    glTexCoord2f(1.0f,0.0f); glVertex3f(-1.25f,0.0f,7.5f);
		glEnd();
}
void Rysujpomieszczenie()
{

//Sufit
	glBindTexture(GL_TEXTURE_2D, sufit);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(14.0f,14.0f); glVertex3f(-7.0f,5.0f,-7.0f);
			glTexCoord2f(0.0f,14.0f); glVertex3f(7.0f,5.0f,-7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(7.0f,5.0f,7.0f);	
		    glTexCoord2f(14.0f,0.0f); glVertex3f(-7.0f,5.0f,7.0f);
		glEnd();

//Ściana tył
	glBindTexture(GL_TEXTURE_2D, sciana);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(4.0f,5.0f); glVertex3f(1.25f,5.0f,7.0f);
			glTexCoord2f(0.0f,5.0f); glVertex3f(7.0f,5.0f,7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(7.0f,0.0f,7.0f);	
		    glTexCoord2f(4.0f,0.0f); glVertex3f(1.25f,0.0f,7.0f);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(4.0f,5.0f); glVertex3f(-7.0f,5.0f,7.0f);
			glTexCoord2f(0.0f,5.0f); glVertex3f(-1.25f,5.0f,7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-1.25f,0.0f,7.0f);	
		    glTexCoord2f(4.0f,0.0f); glVertex3f(-7.0f,0.0f,7.0f);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(2.0f,1.0f); glVertex3f(-1.25f,5.0f,7.0f);
			glTexCoord2f(0.0f,1.0f); glVertex3f(1.25f,5.0f,7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(1.25f,4.0f,7.0f);	
		    glTexCoord2f(2.0f,0.0f); glVertex3f(-1.25f,4.0f,7.0f);
		glEnd();
//Ściana przód
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f,5.0f); glVertex3f(-7.0f,5.0f,-7.0f);
			glTexCoord2f(10.0f,5.0f); glVertex3f(7.0f,5.0f,-7.0f);
			glTexCoord2f(10.0f,0.0f); glVertex3f(7.0f,0.0f,-7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-7.0f,0.0f,-7.0f);
		glEnd();
//Ściana lewa
	glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,5.0f); glVertex3f(-7.0f,5.0f,7.0f);
			glTexCoord2f(12.0f,5.0f); glVertex3f(-7.0f,5.0f,-7.0f);
			glTexCoord2f(12.0f,0.0f); glVertex3f(-7.0f,0.0f,-7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-7.0f,0.0f,7.0f);
		glEnd();
//Ściana prawa
	glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(10.0f,5.0f); glVertex3f(7.0f,5.0f,7.0f);
			glTexCoord2f(0.0f,5.0f); glVertex3f(7.0f,5.0f,-7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(7.0f,0.0f,-7.0f);	
		    glTexCoord2f(10.0f,0.0f); glVertex3f(7.0f,0.0f,7.0f);
		glEnd();
//Wnęka
	glBindTexture(GL_TEXTURE_2D, prog);
	glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,5.5f); glVertex3f(-1.25f,4.0f,7.5f);
			glTexCoord2f(0.5f,5.5f); glVertex3f(-1.25f,4.0f,7.0f);
			glTexCoord2f(0.5f,0.0f); glVertex3f(-1.25f,0.0f,7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-1.25f,0.0f,7.5f);
		glEnd();
			glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,5.5f); glVertex3f(1.25f,4.0f,7.5f);
			glTexCoord2f(0.5f,5.5f); glVertex3f(1.25f,4.0f,7.0f);
			glTexCoord2f(0.5f,0.0f); glVertex3f(1.25f,0.0f,7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(1.25f,0.0f,7.5f);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(3.0f,1.0f); glVertex3f(1.25f,4.0f,7.5f);
			glTexCoord2f(0.0f,1.0f); glVertex3f(-1.25f,4.0f,7.5f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-1.25f,4.0f,7.0f);	
		    glTexCoord2f(3.0f,0.0f); glVertex3f(1.25f,4.0f,7.0f);
		glEnd();
//prog
	glBindTexture(GL_TEXTURE_2D, prog);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(5.75f,0.2f); glVertex3f(1.25f,0.2f,6.99f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(7.0f,0.2f,6.99f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(7.0f,0.0f,6.99f);	
		    glTexCoord2f(5.75f,0.0f); glVertex3f(1.25f,0.0f,6.99f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(5.75f,0.2f); glVertex3f(-7.0f,0.2f,6.99f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(-1.25f,0.2f,6.99f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-1.25f,0.0f,6.99f);	
		    glTexCoord2f(5.75f,0.0f); glVertex3f(-7.0f,0.0f,6.99f);
		glEnd();

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(-7.0f,0.2f,-6.99f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(7.0f,0.2f,-6.99f);
			glTexCoord2f(14.0f,0.0f); glVertex3f(7.0f,0.0f,-6.99f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-7.0f,0.0f,-6.99f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(-6.99f,0.2f,7.0f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(-6.99f,0.2f,-7.0f);
			glTexCoord2f(14.0f,0.0f); glVertex3f(-6.99f,0.0f,-7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-6.99f,0.0f,7.0f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(6.99f,0.2f,7.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(6.99f,0.2f,-7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(6.99f,0.0f,-7.0f);	
		    glTexCoord2f(14.0f,0.0f); glVertex3f(6.99f,0.0f,7.0f);
		glEnd();
		//gora
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(-7.0f,5.0f,6.99f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(7.0f,5.0f,6.99f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(7.0f,4.8f,6.99f);	
		    glTexCoord2f(14.0f,0.0f); glVertex3f(-7.0f,4.8f,6.99f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(-7.0f,5.0f,-6.99f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(7.0f,5.0f,-6.99f);
			glTexCoord2f(14.0f,0.0f); glVertex3f(7.0f,4.8f,-6.99f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-7.0f,4.8f,-6.99f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(-6.99f,5.0f,7.0f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(-6.99f,5.0f,-7.0f);
			glTexCoord2f(14.0f,0.0f); glVertex3f(-6.99f,4.8f,-7.0f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-6.99f,4.8f,7.0f);
		glEnd();
	glBegin(GL_QUADS);
		glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(14.0f,0.2f); glVertex3f(6.99f,5.0f,7.0f);
			glTexCoord2f(0.0f,0.2f); glVertex3f(6.99f,5.0f,-7.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(6.99f,4.8f,-7.0f);	
		    glTexCoord2f(14.0f,0.0f); glVertex3f(6.99f,4.8f,7.0f);
		glEnd();
}
void Rysujpoodloge()
{
//Podłoga
	glBindTexture(GL_TEXTURE_2D, podloga);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f,10.0f); glVertex3f(-7.0f,0.0f,-7.0f);
			glTexCoord2f(11.0f,10.0f); glVertex3f(7.0f,0.0f,-7.0f);
			glTexCoord2f(11.0f,0.0f); glVertex3f(7.0f,0.0f,7.5f);	
		    glTexCoord2f(0.0f,0.0f); glVertex3f(-7.0f,0.0f,7.5f);
		glEnd();
	
}
void Napisnaekranie()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING);
	glColor3f(0.0f,1.0f,0.0f);
	glRasterPos3f(-0.035f,0.02f,-0.1f);
	info();
	glPrint(napis);
	//"odleglosc=%6.2f",odleglosc1
	
	glColor3f(1.0f,1.0f,1.0f);
	glEnable( GL_LIGHTING);
}
void modele(){	
	glBindTexture(GL_TEXTURE_2D, marmur);
	glTranslatef(0.0f, 1.55f, -4.25f);
	glScalef(8.0f,8.0f,8.0f);	
	glCallList( model1 );
}
void informator(){
	glDisable( GL_LIGHTING);
	glTranslatef(0.0f, 0.0f, -2.5f);
	glBindTexture(GL_TEXTURE_2D, textura);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f,1.0f); glVertex3f(1.4f,1.1f,0.0f);
			glTexCoord2f(0.0f,1.0f); glVertex3f(-1.4f,1.1f,0.0f);
			glTexCoord2f(0.0f,0.0f); glVertex3f(-1.4f,-1.1f,0.0f);	
		    glTexCoord2f(1.0f,0.0f); glVertex3f(1.4f,-1.1f,0.0f);
		glEnd();
	glEnable( GL_LIGHTING);
}
//Rysowanie całej sceny
int Rysowaniesceny(GLvoid)									
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Czyści ekran i bufor głębi
	glLoadIdentity();									// resetuje widok
	
	if ((napis != "") && enter){
		if (textura !=0)
		{
			informator();
		}
		else
		{
			wylacz=TRUE;
		}
	}else{
//chodzenie
	kolizje();
	glRotatef(Tilt,1.0f,0,0);
	glRotatef(Heading,0,1.0f,0);
	glTranslatef(X, - 1.8 + HeadMovement , Z);	
	
	Rysujpomieszczenie();
	obrazy();
	modele();
	swiatlo();


	glLoadIdentity();
	glDisable( GL_LIGHTING);
	glRotatef(Tilt,1.0f,0,0);
	glRotatef(Heading,0,1.0f,0);
	glTranslatef(X, - 1.8 + HeadMovement , Z);
	glScalef(1.0, -1.0, 1.0);
	
	Rysujpomieszczenie();
	obrazy();
	modele();
	
	glLoadIdentity();									// resetuje widok
	glRotatef(Tilt,1.0f,0,0);
	glRotatef(Heading,0,1.0f,0);
	glTranslatef(X, - 1.8 + HeadMovement , Z);	
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.15f);					// przezroczystość 15%.
	
	Rysujpoodloge();
	
	glDisable(GL_BLEND);
	
	Napisnaekranie();
	}
    return TRUE;// Udało się wszystko narysować
}

GLvoid KillGLWindow(GLvoid)								
{
	if (fullscreen)                             // Fullscreen on?
	{
		ChangeDisplaySettings(NULL,0);          // To do okienka
		ShowCursor(TRUE);                       // i pokaż kursor
	}
	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Nie udało się zwolnić urzędzeń i zakończyć renderingu.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Nie udało się zakończyć renderingu.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}
		if (hDC && !ReleaseDC(hWnd,hDC))				
	{
		MessageBox(NULL,"Nie udało się zwolnić urzędzeń.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Nie udało się zamknąć okienka.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("Projekt",hInstance))			
	{
		MessageBox(NULL,"Nie udało się wyrejestrować klasy.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}
/*	Tworzenie okna
 tytuł
 szerokość
 wysokość
 ilość bitów kolorów
 fullscreen
 */
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Format pixeli
	WNDCLASS	wc;						// Struktura klasy okna
	DWORD		dwExStyle;				// Styl okna
	DWORD		dwStyle;				// Styl okna
	RECT		WindowRect;				
	WindowRect.left=(long)0;			// pozycja okna na 0,0
	WindowRect.right=(long)width;		// lewy górny róg
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		
	ShowCursor(FALSE);
	fullscreen=fullscreenflag;			// Jak tam Fullscreen mode

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// Menu, nie dzęki
	wc.lpszClassName	= "Projekt";							// Set The Class Name

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Nie można zarejestrować klasy.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.

			if (MessageBox(NULL,"Twoja karta graficzna nie obsługuje danej rozdzielczości.\n Pozostać w aktualnym trybie?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Za chwilę następi zamknięcie programu.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Fullscreen?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Styl okna
		dwStyle=WS_POPUP;										// 
		ShowCursor(FALSE);										// Ukrycie kursora
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Tworzenie okna
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"Projekt",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Problem z utworzeniem okna.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Nie można nawiązać kontaktu z urządzeniem.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Nie odnaleziono właściwego formatu pixeli.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Nie można ustawić właściwego formaty pixeli.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Nie można zrenderować.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Nie można uruchomić renderingu.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	Skalowanie(width, height);					// Set Up Our Perspective GL Screen

	if (!Ustawienia())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Problem z ustawieniami.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;										// Success
}


//Sprawdza informacji z urządzeń (klawiatura, okna ,itp.)
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Sprawdzanie widomości z windowsa
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		
		case WM_SIZE:								// Resize The OpenGL Window
		{
			Skalowanie(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		
		}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
//Main czyli główa część
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	
	// W jakim trybie odpalić program
	if (MessageBox(NULL,"Włączyć w trybie pełnoekranowym?", "Tryb pełnoekranowy.",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Window Mode on
	}

// Tworzenie okna
	if (!CreateGLWindow("Wirtualna wystawa rzeźby. Autor: Wrześniacki Robert",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	SetCursorPos( 400, 300 );
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else
		{
// obsługa ruchu myszki
			if ( !GetForegroundWindow())
			{
			}
			else
			{
				GetCursorPos(&mpos);
				SetCursorPos( 400, 300 );
				Heading = Heading + double( ( mpos.x - 400 ) ) / 100 * MouseSpeed;
				Tilt = Tilt - double( ( 300 - mpos.y ) ) / 100 * MouseSpeed;
				if ( Tilt > 60 )
				  Tilt = 60;
				if ( Tilt < - 60 )
				  Tilt = - 60;
			}
	
//Rysuje scene i sprawdza czy nie wciśnięto esc, albo nie ma info z Rysowaniesceny() o zamknięciu
			if ((active && !Rysowaniesceny()) || keys[VK_ESCAPE] || wylacz)
			{
				done=TRUE;							
			}
//Obsługa klawiszy			
			else									
			{
				SwapBuffers(hDC);					

				if (keys[VK_RETURN] && !ent)
				{
					ent=TRUE;
					enter2=!enter2;
					if (!enter2)
					{
						enter = FALSE;
					}
					else
					{
						enter = TRUE;
					}
				}
				if (!keys[VK_RETURN])
				{
					ent=FALSE;
				}

				if ( keys['O'])
				{
					jasno=jasno+0.01;
				}
				if ( keys['P'])
				{
					jasno=jasno-0.01;
				}
				if ( keys['L'])
				{
					jasno=0.58;
				}
			
				lastZ=Z;
				lastX=X;
				if ( keys[VK_UP] || keys['W'])
				{
					X -= double( sin( double( Heading ) * piover180 ) ) * 0.05f;  
					Z = Z + double( cos( double( Heading ) * piover180 ) ) * 0.05f;
					HeadMovAngle = HeadMovAngle + 5;
					HeadMovement = 0.025 * sin( double( HeadMovAngle ) * piover180 );
				}
				if ( keys[VK_DOWN] || keys['S'])
				{
					X += double( sin( double( Heading ) * piover180 ) ) * 0.05f;
					Z = Z - double( cos( double( Heading ) * piover180 ) ) * 0.05f;
					HeadMovAngle = HeadMovAngle - 5;
					HeadMovement = 0.025 * sin( double( HeadMovAngle ) * piover180 );
				}
				if ( keys[VK_LEFT] || keys['A'])
				{
					X = X + double( sin( double( ( Heading + 90 ) ) * piover180 ) ) * 0.05f;
					Z -= double( cos( double( ( Heading + 90 ) ) * piover180 ) ) * 0.05f;  
				}
				if ( keys[VK_RIGHT] || keys['D'])
				{
					X = X - double( sin( double( ( Heading + 90 ) ) * piover180 ) ) * 0.05f;
					Z += double( cos( double( ( Heading + 90 ) ) * piover180 ) ) * 0.05f;
				}

				if (keys[VK_F1])						
				{
					keys[VK_F1]=FALSE;					
					KillGLWindow();						// Zamykanie okna
					fullscreen=!fullscreen;				// Fullscreen on/off
					// odtworzenie okna
					if (!CreateGLWindow("Wirtualna wystawa rzeźby. Autor: Wrześniacki Robert",640,480,16,fullscreen))
					{
						return 0;						// Zamknij jeśli okno się nie utworzyło
					}
				}
			}
		}
	}

// Zamykanie
	KillGLWindow();										
	return (msg.wParam);								
}
