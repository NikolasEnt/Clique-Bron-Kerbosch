#include <stdlib.h>  //needed for exit function
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <GL/glfw.h> //include header file for glfw library so that we can use OpenGL
#include <GL/glu.h>

typedef unsigned char byte;
typedef unsigned long long ull;

using namespace std;
int Nmax = 10;
int ent_counter=0;


#define Nmax 64 // Set the grid size limit
byte N, count;
ull pik[Nmax],clique[Nmax],mask;

//Graphics
#define scale 1.6f
#define num_scale 1.1f
#define PI2 6.2831853071
#define dp -5.0f

double dots[65][2];

void init_dots(){
    float dang=PI2/Nmax;
    float ang=PI2/4;
    for (int i=0; i<Nmax; i++){
        dots[i][0]=scale*cos(ang);
        dots[i][1]=scale*sin(ang);
        ang+=dang;
    }
    dots[Nmax][0]=dots[0][0];
    dots[Nmax][1]=dots[0][1];
}

//Called when a key is pressed
void GLFWCALL handleKeypress(int key,int press) //The key that was pressed
{
    switch (key) {
        case GLFW_KEY_ESC: //Escape key
            exit(0); //Exit the program
        case GLFW_KEY_ENTER:
            ent_counter++;
    }
}

//Called when the window is resized
void GLFWCALL handleResize(int width,int height)
{
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
    //Set the camera perspective
    glLoadIdentity(); //reset the camera
    gluPerspective( 45.0f,                      //camera angle
                (GLfloat)width/(GLfloat)height, //The width to height ratio
                 1.0f,                          //The near z clipping coordinate
                100.0f );                       //The far z clipping coordinate
}

//Initializes rendering
void initializeRendering()
{
    glfwInit();
    init_dots();
     if( !glfwOpenWindow( 768, // width of window
                          768, //height of window
                          1,  //redbits
                          0,  //greenbits
                          0,  //bluebits
                          0,  //alphabits
                          0,  //depthbits
                          0, //stencilbits
                          GLFW_WINDOW ) //mode
        ) //return false if window is not created
    {
        glfwTerminate(); //terminating glfw window

    }

    glfwSetWindowTitle("Clique finder");
    glfwSetWindowSizeCallback(handleResize); //callback function of GLFW to handle window resize
    glfwSetKeyCallback(handleKeypress); //callback function to handle keypress
    glPointSize(7.0f);
}

void connect_dots(int i, int j, bool color){
    if (color){
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    else{
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    glBegin(GL_LINES);
    glVertex3f( dots[i][0], dots[i][1], dp );
    glVertex3f( dots[j][0], dots[j][1], dp );
    glEnd();
}

void draw_graph(){
    glLineWidth(0.5f);
    glColor3f(0.0f, 0.0f, 0.0f);
    //glEnable(GL_LINE_SMOOTH);
    for (int i=0; i<Nmax;i++){
        for (int j=0; j<Nmax; j++){
            connect_dots(i, j, true);
        }
        //connect_dots(i, i+1, true);
        glBegin(GL_POINTS);
        if (i==0){
            glColor3f(0.0f, 1.0f, 0.7f);
        }
        else{
            glColor3f(0.0f, 0.0f, 0.0f);
        }
        glVertex3f(dots[i][0], dots[i][1], dp);
        glEnd();

    }
}


void display()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f ); //clear background screen to white
    //Clear information from last draw
    glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective

    draw_graph();


    glfwSwapBuffers();

}


//End of graphics



//	Counting the number of ones in the binary representation of a number
byte number( ull x)
{
	byte i;
	ull single, ret;
	single=1;
	ret=0;
	for (i=0;i<Nmax;i++)
	{
		ret=ret+((x&single)>>i);
		single=single<<1;
	}
	return ret;
}

// Output of the first N bits of a binary representation of a number
void long_print(ull x, byte N, byte eol)
{
	byte i;
	ull single;
	single=1;
	printf(" ");
	for (i=0;i<N;i++)
	{
		printf("%llu",(x&(single<<(N-1-i)))>>(N-1-i));
	}
	if (eol)
	{
		printf("\n");
	}
}


// The Bron–Kerbosch algorithm
void BronKerbosch(ull R, ull P, ull X)
{
byte i;
ull v,single;
if ((P==0)&&(X==0))
{
	printf("Clique\n");
	long_print(R,N,1);
	clique[count]=R;
	count++;
}
else
{
	single=1;
	for (i=0;i<N;i++)
	{
		v=single<<(N-1-i);
		if(P&v)
		{
			printf("Vertex Cond0 Cond1 Pik\n");
			long_print(v,N,0);
			long_print(P,N,0);
			P=P^v;
			long_print(P,N,0);
			long_print(pik[i],N,1);
			BronKerbosch(R|v,P&pik[i],X&pik[i]);
			X=X|v;
		}
	}
}
return;
}


int main(int argc, char *argv){
FILE *iFile, *oFile;
byte  i, j, bit, max_count;
ull sum,lbit,R,P,X,max;

// Check access to the I / O files.
if ( argc != 3 )
{
	printf( "Usage: %s Inputfile Outputfile.\n", argv[0] );
	return 1;
}
else
{
// 	We assume argv[1] is a filename to open
	iFile = fopen( argv[1], "r" );
	if ( iFile == 0 )
	{
		printf( "Could not open input file\n" );
		return 2;
	}
	oFile = fopen( argv[2], "w" );
	if ( oFile == 0 )
	{
		printf( "Could not open output file\n" );
		return 3;
	}
}

// Read the input file
fscanf (iFile,"%hhu",&N);
if (N>Nmax)
{
	printf("N is larger then 64");
	exit (1);
}

lbit=1;
mask=0;
for (i=0;i<N;i++)
{
	mask=mask|(lbit<<i);
}

// Construct the connection matrix
for (i=0;i<N;i++)
{
	sum=0;
	for (j=0;j<N;j++)
	{
		if (j<i)
		{
//			Use the symmetry of the connection
			lbit=1;
			bit=0;
			if ((pik[j]&(lbit<<(N-i-1)))>0)
			{
				bit=1;
			}
		}
		if (j==i)
		{
//			We assume that the vertex is always not connected with itself
			bit=0;
		}
		if (j>i)
		{
//			Read the input file
			fscanf (iFile,"%hhu",&bit);
			if(bit>1 || bit <0)
			{
				printf("Bad data.");
				exit (2);
			}
		}
		sum=(sum<<1)|bit;
	}
	pik[i]=sum;
}

R=0;
P=mask;
X=0;
count=0;

BronKerbosch(R,P,X);

// Display the largest groups
fprintf (oFile,"Full connected group:\n");

// Find the maximal clique
max=0;
for (i=0;i<count;i++)
{
	if(number(clique[i])>max)
	{
		max=number(clique[i]);
	}
}

// Output the maximal clique
max_count=0;
for (i=0;i<count;i++)
{
	if(number(clique[i])==max)
	{
		max_count++;
		lbit=1;
		for (j=0;j<N;j++)
		{
			if ((clique[i]&(lbit<<(N-(j+1))))>0)
			{
				fprintf(oFile,"%d ",j+1);
			}
		}
		fprintf(oFile,"\n");
	}
}
fprintf (oFile,"Total %d groups.\n",max_count);
fclose(iFile);
fclose(oFile);
return 0;
}

int main()
{
    bool    running = true;
    initializeRendering();



    while(running) // infinite loop to draw object again and again
    {              // because once object is draw then window is terminated
        display();
        running = glfwGetWindowParam( GLFW_OPENED ); //when glfw window is opened then it return true
                                                     //if closed then return false
    }
    return 0;
}








