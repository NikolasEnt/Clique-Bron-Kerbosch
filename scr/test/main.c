#include <stdio.h>
#include <stdlib.h>

#define Nmax 64 // Set the grid size limit

typedef unsigned char byte;
typedef unsigned long long ull;

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

byte N, count;
ull pik[Nmax],clique[Nmax],mask;
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

int main(argc, argv)
int argc;
char *argv[];
{
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
printf("count = %d\n", count);
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
