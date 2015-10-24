#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
FILE *fd;
char *bigbuf;
char header[256];
char trackbuf[6400];
int32_t sides, tracks, sectors, geometry=1;
int gap1,gap2,gap3;
char old_signature[]="ORICDISK";
char new_signature[]="MFM_DISK";

void init_track(int n);
void compute_crc(unsigned char *ptr,int count);

/* TODO:Fix endianness! */
void fileread( void * ptr, size_t size, size_t count, FILE * stream )
{
  if (fread(ptr,size,count,stream)!=count)
  {
    printf("Read error\n");
    exit(EXIT_FAILURE);
  }
}


int main(int argc,char *argv[])
{
  int i,s,t,offset;

  printf("Old -> MFM v1.2\n");
  if (argc!=2)
  {
    printf("Usage: oric2mfm <diskimage>\n");
    printf("(converts old Euphoric disk image format to new MFM format)\n");
    exit(EXIT_FAILURE);
  }
  fd=fopen(argv[1],"r+b");
  if (fd==NULL)
  {
    printf("Unable to open %s for read/write operation\n",argv[1]);
    exit(EXIT_FAILURE);
  }
  fileread(header,8,1,fd);
  if (strncmp(header,old_signature,8)!=0) 
  {
    printf("%s is not an old disk image\n",argv[1]);
    exit(EXIT_FAILURE);
  }
  fileread(&sides,1,sizeof(int32_t),fd);
  fileread(&tracks,1,sizeof(int32_t),fd);
  fileread(&sectors,1,sizeof(int32_t),fd);
  fileread(header+20,256-20,1,fd);
  if ( (sides>2) || (tracks>256) || (sectors>18) )
  {
    printf("A valid floppy cannot have %d sides, %d tracks and %d sectors per track\n",sides,tracks,sectors);
    exit(EXIT_FAILURE);
  }
  bigbuf = malloc(sides*tracks*sectors*256);
  if (bigbuf==NULL)
  {
    printf("Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  fileread(bigbuf,sides*tracks*sectors,256,fd);
  fseek(fd,0,SEEK_SET);
  /*printf("read old.\n");*/

  switch (sectors) 
  {
  case 15: case 16: case 17:
    gap1=72; gap2=34; gap3=50;
    break;
  case 18:
    gap1=12; gap2=34; gap3=46;
    break;
  default:
    printf("unrealistic sectors per track number\n");
    exit(EXIT_FAILURE);
  }
  init_track(sectors);

  /*printf("writing header...\n");*/
  fwrite(new_signature,8,1,fd);
  fwrite(&sides,1,sizeof(int32_t),fd);
  fwrite(&tracks,1,sizeof(int32_t),fd);
  fwrite(&geometry,1,sizeof(int32_t),fd);
  fwrite(header+20,256-20,1,fd);

  for(s=0;s<sides;s++)
    for(t=0;t<tracks;t++)
    {
      offset=gap1;
      for(i=0;i<sectors;i++)
      {
        /*printf("writing size %d track %d sector %d...\n", s, t, i);*/
        trackbuf[offset+4]=t;
        trackbuf[offset+5]=s;
        trackbuf[offset+6]=i+1;
        trackbuf[offset+7]=1;
        compute_crc(trackbuf+offset,4+4);
        offset+=4+6;
        offset+=gap2;
        memcpy(trackbuf+offset+4,bigbuf+((s*tracks+t)*sectors+i)*256,256);
        compute_crc(trackbuf+offset,4+256);
        offset+=256+6;
        offset+=gap3;
      }
      fwrite(trackbuf,6400,1,fd);
    }
    /*printf("done.\n");*/
    free(bigbuf);
    return EXIT_SUCCESS;
}

void init_track(int n)
{
  int i,j,offset=0;

  for (i=0;i<gap1-12;i++) trackbuf[offset++]=0x4E;
  for (j=0;j<n;j++)
  {
    for (i=0;i<12;i++) trackbuf[offset++]=0;
    for (i=0;i<3;i++) trackbuf[offset++]=0xA1;
    trackbuf[offset++]=0xFE;
    for (i=0;i<6;i++) offset++;
    /*                for (i=0;i<gap2-12;i++) trackbuf[offset++]=0x4E;
    */ for (i=0;i<gap2-12;i++) trackbuf[offset++]=0x22;
    for (i=0;i<12;i++) trackbuf[offset++]=0;
    for (i=0;i<3;i++) trackbuf[offset++]=0xA1;
    trackbuf[offset++]=0xFB;
    for (i=0;i<258;i++) offset++;
    for (i=0;i<gap3-12;i++) trackbuf[offset++]=0x4E;
  }
  while (offset<6400) trackbuf[offset++]=0x4E;
}

unsigned int crctab[256] =
{
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

void compute_crc(unsigned char *ptr,int count)
{
  int i;
  unsigned short crc=0xFFFF,byte;
  for (i=0;i<count;i++)
  {
    byte= *ptr++;
    crc=(crc<<8)^crctab[(crc>>8)^byte];
  }
  *ptr++=crc>>8;
  *ptr++=crc&0xFF;
}
