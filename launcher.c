#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "data_creator/data_creator.h"

typedef struct LauncherArguments
{
    unsigned int command; //0 - generate, 1 - encrypt, 2 - decrypt
    const char* input_file_name;
    const char* output_file_name;
    unsigned int seed;
    unsigned long qnt;
} LauncherArguments;

void PrintHelp(FILE* stream, const char* program_name, int exit_code)
{
    fprintf (stream, "Usage: %s options [ inputfile ... ]\n", program_name);
    fprintf (stream,
            "  -h  --help             Display this usage information.\n"
            "  -g  --generate         Generate syntetic data to encrypt.\n"
            "  -e  --encrypt          Encrypt input file (-i) and save result to output (-o).\n"
            "  -d  --decrypt          Decrypt input file (-i) and save result to output (-o).\n"
            "  -i  --input filename   Read data from file.\n "
            "  -o  --output filename  Write output to file.\n "
            "  -q  --quantity         Quantity of records that should be generated.\n "
            "  -s  --seed             Define seed number [1-4294967296] for randon oprtation (for determination behavior).\n "
            "  -v  --verbose          x3 for now.\n");
    exit (exit_code); 
}

void ParseArguments(int argc, char** argv, LauncherArguments* args)
{
    const char* program_name = argv[0];
    *args = (LauncherArguments) { .command = 999, .input_file_name = NULL, .output_file_name = NULL, .qnt = 100, .seed = 0 };

    const char* const short_options = "gedhvo:i:q:s:";
    const struct option long_options[] = {
        { "help",     0, NULL, 'h' },
        { "generate", 0, NULL, 'g' },
        { "encrypt",  0, NULL, 'e' },
        { "decrypt",  0, NULL, 'd' },
        { "output",   1, NULL, 'o' },
        { "input",    1, NULL, 'i' },
        { "quantity", 1, NULL, 'q' },
        { "seed",     1, NULL, 's' },
        { "verbose",  0, NULL, 'v' },
        { NULL,       0, NULL,  0  }
    };

    int verbose = 0;
    int next_option;

    do {
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);

        switch (next_option)
        {
        case 'g': // generate
            args->command = 0;
            break;

        case 'e': // encrypt
            args->command = 1;
            break;

        case 'd': // decrypt
            args->command = 2;
            break;

        case 'h': // -h or --help
            PrintHelp (stdout, program_name, 0);
        
        case 'o':
            //output_file = optarg;
            args->output_file_name = optarg;
            break;

        case 'i':
            args->input_file_name = optarg;
            break;
        
        case 'v':
            verbose = 1;
            break;

        case 'q':
            args->qnt = (unsigned int )strtoul(optarg, NULL, 10);
            break;

        case 's':
            args->seed = strtoul(optarg, NULL, 10);
            break;

        case '?':
            PrintHelp(stderr, program_name, 1);

        case -1:
            break;

        default: 
            abort();
            break;
        }
    }
    while (next_option != -1);

    if (verbose)
    {
        printf("Parsed arguments:\n");
        printf("Command = %d\n", args->command);
        printf("Input = %s\n", args->input_file_name);
        printf("Output = %s\n", args->output_file_name);        
        printf("Quantity = %lu\n", args->qnt);
        printf("Quantity = %u\n", args->seed);
    }

    //return 0;
}

int main (int argc, char *argv[]) 
{
    LauncherArguments args;
    ParseArguments(argc, argv, &args);

    if (args.command == 0)
        GenerateData(args.input_file_name, args.qnt, args.output_file_name, args.seed);
    else if (args.command == 1)
        printf ("Encryption to be implemented.\n");
    else if (args.command == 2)
        printf ("Decryption to be implemented.\n");
    else
        printf ("Undefined command. Please specify -g, -e or -d.\n");

    return 0;
}