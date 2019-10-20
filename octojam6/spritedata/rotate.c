#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>


int readDataBit(uint8_t* data, int wBytes, int hBytes, int xBitRead, int yBitRead)
{
	int byteOffsetToRead = yBitRead * wBytes;
	byteOffsetToRead += xBitRead / 8;
	// printf("boRow = %d, boCol = %d, ", yBitRead * wBytes, xBitRead / 8);

	int bitsToShift = 7 - (xBitRead % 8);

	int retVal = ( data[byteOffsetToRead] >> bitsToShift ) & 0x1;

	// printf("offset = %d, bitsToShift = %d, retVal = %d: ", byteOffsetToRead, bitsToShift, retVal);

	return retVal;
}

void printUsage(char* appName)
{
	printf("%s -h        # For help\n", appName);
	printf("%s widthBytes inputFilename outputFilename      # Rotate bits in a file\n", appName);
}



int main(int argc, char** argv)
{
	if (argc != 4)
	{
		printUsage(argv[0]);
		return 1;
	}

	// Validate the width value
	int inputFileWidth = atoi(argv[1]);
	if (inputFileWidth < 1)
	{
		printf("Error.  width value %s is invalid\n", argv[1]);
		return 2;
	}

	int inputFileFd;
	int outputFileFd;

	inputFileFd = open(argv[2], O_RDONLY);
	if (inputFileFd == -1)
	{
		printf("Error opening file %s\n", argv[3]);
		printf("Error = %s\n", strerror(errno));
		return 2;
	}

	printf("Opened file %s for reading\n", argv[3]);

	outputFileFd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputFileFd == -1)
	{
		printf("Error opening the output file %s\n", argv[3]);
		printf("Error = %s\n", strerror(errno));
		goto error_close_input_file;
	}

	printf("Opened file %s for writing\n", argv[3]);

	// Find out how many bytes are in the input file
	off_t inputFileSizeBytes = lseek(inputFileFd, 0, SEEK_END);
	if (inputFileSizeBytes == -1)
	{
		printf("Error determining the file size of the input file\n");
		printf("  Error = %s\n", strerror(errno));
		goto error_close_output_files;
	}

	if (-1 == lseek(inputFileFd, 0, SEEK_SET))
	{
		printf("Error  determining the file size of the input file (rewind step)\n");
		printf("  Error = %s\n", strerror(errno));
		goto error_close_output_files;
	}

	printf("Input file size is %ld\n", inputFileSizeBytes);
	int inputFileHeight = inputFileSizeBytes / inputFileWidth;
	printf("Input file width x height (bytes) = %d x %d\n", inputFileWidth, inputFileHeight);
	// Allocate memory for the entire file and read it into memory
	uint8_t* inputFileData = (uint8_t*) malloc(inputFileSizeBytes);
	int bytesReadTotal = 0;
	while(bytesReadTotal < inputFileSizeBytes)
	{
		int br = read(inputFileFd, inputFileData + bytesReadTotal, inputFileSizeBytes - bytesReadTotal);
		if (br == -1)
		{
			printf("Error reading the contents of the file at byte %d\n", bytesReadTotal);
			printf("  Error = %s\n", strerror(errno));
			goto error_close_output_files;
		}

		bytesReadTotal += br;
	}

	printf("Read %d bytes\n", bytesReadTotal);

	printf("What we read:\n");
	for(int row = 0; row < inputFileHeight; row++)
	{
		for(int col = 0; col < inputFileWidth * 8; col++)
		{
			if (readDataBit(inputFileData, inputFileWidth, inputFileHeight, col, row))
			{
				printf("#");
			}
			else
			{
				printf(" ");
			}
		}

		printf("\n");
	}
	
	printf("\n\nWhat we are going to write:\n");

	uint8_t curByteToWrite = 0;
	int bytesWritten = 0;
	int curBit = 0;
	for(int row = 0; row < inputFileWidth * 8; row++)
	{
		for(int col = 0; col < inputFileHeight; col++)
		{
			int readRow = col;
			int readCol = inputFileWidth * 8 - 1 - row;
			int bitRead = readDataBit(inputFileData, inputFileWidth, inputFileHeight, readCol, readRow);
			if (bitRead)
			{
				printf("#");
			}
			else
			{
				printf(" ");
			}

			int bitsToShift = 7 - curBit;
			curByteToWrite += bitRead << bitsToShift;

			if (bitsToShift == 0)
			{
				// Full byte has been created, write it to the output file!
				if (1 != write(outputFileFd, &curByteToWrite, 1))
				{
					// We failed to write
					printf("Error trying to write the byte %d\n", bytesWritten);
					printf("  Error = %s\n", strerror(errno));
					goto error_close_output_files;
				}

				curBit = 0;
				curByteToWrite = 0;
				bytesWritten++;
			}
			else
			{
				curBit++;
			}
		}

		printf("\n");
	}
		
		
	// If the input file is a file (and not stdin), close it
	printf("Closing intput file\n");
	close(inputFileFd);

	// Close the output files
	printf("Closing output file\n");
	close(outputFileFd);
	
	return 0;

error_close_output_files:
	
	printf("Closing output file\n");
	close(outputFileFd);

error_close_input_file:

	printf("Closing intput file\n");
	close(outputFileFd);

	return 4;
}
