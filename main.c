#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int hexToInt(unsigned char* number){
	return (int) (number[1]<< 8 | number[0]);
}
void  isEquals(unsigned short* fat0,unsigned short* fat1, int fatClusterSize){
	int i=0;
	for(i=3;i<fatClusterSize;i++){
		if(fat0[i]!=fat1[i]){
			printf("DIFF %d:%d,%d",i,fat0[i],fat1[i]);
		}

	}

}
void blocosRemovidos(unsigned short* fat, int fatSize, int numBytePos, FILE* file){
	int i = 0;
	unsigned char entry[32];
	fseek(file,numBytePos,SEEK_SET);
	for(i=0;i<fatSize;i++){
		fread(entry,32,1,file);
		if(entry[0]==229 && fat[i]!=0){
			printf("REMOVIDOS %d ",i);
		}
	}
	printf("\n");

}
int main(int argc, char* argv[] ){
	FILE *file;
	if(!(file = fopen("disco2","r+"))){
		printf("Falha ao abrir o arquivo");
		exit(1);
	}
	unsigned char reservedSectors[2];
	unsigned char bytesSector[2];
	unsigned char numSectorFat[2];
	unsigned char numFatCopies[1];
	unsigned char numRootDirectories[2];

	fseek(file,11,SEEK_CUR);
	fread(bytesSector,2,1,file);
	//printf("%d \n",hexToInt(bytesSector));
	fseek(file,1,SEEK_CUR);
	fread(reservedSectors,2,1,file);
	//printf("%d \n",hexToInt(reservedSectors));
	fread(numFatCopies,1,1,file);
	//printf("%d \n",numFatCopies[0]);
	fread(numRootDirectories,2,1,file);
	//printf("%d \n",hexToInt(numRootDirectories));
	fseek(file,3,SEEK_CUR);
	fread(numSectorFat,2,1,file);
	//printf("%d \n",hexToInt(numSectorFat));

	int reservedRegion = hexToInt(reservedSectors) * hexToInt(bytesSector);
	int fatSize = hexToInt(numSectorFat) * hexToInt(bytesSector);
	int fatClusterSize = fatSize/2;
	int fat0InitialPosition = reservedRegion;
	int fat1InitialPosition = reservedRegion+ fatSize;
	fseek(file,fat0InitialPosition,SEEK_SET);
	unsigned short fat0[fatClusterSize];
	unsigned short fat1[fatClusterSize];
	fread(fat0,fatClusterSize,2,file);
	fread(fat1,fatClusterSize,2,file);
	//Posicao onde comecam os diretorios
	int rootPos = hexToInt(reservedSectors) + (numFatCopies[0] * hexToInt(numSectorFat));
	int dataPos = rootPos + ((hexToInt(numRootDirectories)*32)/hexToInt(bytesSector));
	int numBytesPos =  rootPos * hexToInt(bytesSector);
	//printf("%zx \n" , fat0[0]);
	//printf("%zx \n" , fat1[0]);
	isEquals(fat0,fat1,fatClusterSize);
	blocosRemovidos(fat0,fatSize,numBytesPos,file);

}
