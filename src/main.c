
#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "debug.h"
#include "generation.h"
#include "xmalloc.h"

FILE *inputfile = NULL;
FILE *outputtempfile = NULL;
FILE *outputfile = NULL;

void closefiles(void)
{
	if (inputfile)
		fclose(inputfile);
	if (outputtempfile)
		fclose(outputtempfile);
	if (outputfile && outputfile != stdin && outputfile != stdout &&
	    outputfile != stderr)
		fclose(outputfile);
}

void open_files(void)
{
	logger(DEBUG, no_error, "Opening files");

	outputtempfile = tmpfile();
	if (!outputtempfile) {
		logger(ERROR, error_system, "Unable to create temporary file");
		exit(EXIT_FAILURE);
	}

	logger(DEBUG, no_error, "Opening %s", *cmdargs.inputfile->filename);
#ifdef __STDC_LIB_EXT1__
	if (fopen_s(&inputfile, *cmdargs.inputfile->filename, "r"))
#else
	inputfile = fopen(*cmdargs.inputfile->filename, "r");
	if (!inputfile)
#endif
	{
		logger(ERROR, error_system, "Unable to open input file");
		exit(EXIT_FAILURE);
	}

	if (!**cmdargs.outputfile->filename) {
		logger(DEBUG, no_error, "All files opened successfully");
		outputfile = stdout;
		return;
	}

	logger(DEBUG, no_error, "Opening %s", *cmdargs.outputfile->filename);
#ifdef __STDC_LIB_EXT1__
	if (fopen_s(&outputfile, *cmdargs.outputfile->filename, "wb"))
#else
	outputfile = fopen(*cmdargs.outputfile->filename, "wb");
	if (!outputfile)
#endif
	{
		logger(ERROR, error_system, "Unable to open output file");
		perror("Error: ");
		exit(EXIT_FAILURE);
	}

	logger(DEBUG, no_error, "All files opened successfully");
}

void copy_files(FILE *dest, FILE *src)
{
	const long pos = ftell(src);
	rewind(outputtempfile);

	char *buffer = xmalloc(BUFSIZ);
	for (;;) {
		const size_t bytes = fread(buffer, 1, BUFSIZ, src);
		fwrite(buffer, 1, bytes, dest);
		if (bytes != BUFSIZ)
			break;
	}
	free(buffer);

	fseek(src, pos, SEEK_SET);
}

int main(int argc, char *argv[])
{
	parse_cmdargs(argc, argv);
	open_files();
	parse_file(inputfile, outputtempfile);

	logger(DEBUG, no_error, "Done generating bytecode");
	copy_files(outputfile, outputtempfile);
	logger(DEBUG, no_error, "Finished writing bytecode to output");
	closefiles();

	return get_clean_exit(ERROR);
}
