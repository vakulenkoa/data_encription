#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <libgen.h>

#include <dlfcn.h>

#include "data_creator/data_creator.h"

#define DATA_CREATOR_LIB_FILE_NAME "./libdata_creator.so"
#define GEN_DATA_FUNC_NAME "GenerateData"

#define LONG_PATH_BUFF_SIZE 384

typedef struct LauncherArguments
{
    unsigned int command; //0 - generate, 1 - encrypt, 2 - decrypt
    const char* input_file_path;
    const char* output_file_path;
    unsigned int seed;
    unsigned long qnt;
    const char* secret_phrase;
    const char* encrypt_key_file_path;
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
            "  -s  --seed             Define seed number [1-4294967296] for randon oprtation (for determination behavior).\n"
            "  -p  --phrase           Secret phrase for encryption/decription.\n"
            "  -k  --key              Encryption key file path.\n"
            "  -v  --verbose          X3 for now.\n");
    exit (exit_code); 
}

void ParseArguments(int argc, char** argv, LauncherArguments* args)
{
    const char* program_name = argv[0];
    *args = (LauncherArguments) { .command = 999, .input_file_path = NULL, .output_file_path = NULL, .qnt = 100, .seed = 0, .secret_phrase = NULL, .encrypt_key_file_path = NULL };

    const char* const short_options = "gedhvo:i:q:s:p:k:";
    const struct option long_options[] = {
        { "help",     0, NULL, 'h' },
        { "generate", 0, NULL, 'g' },
        { "encrypt",  0, NULL, 'e' },
        { "decrypt",  0, NULL, 'd' },
        { "output",   1, NULL, 'o' },
        { "input",    1, NULL, 'i' },
        { "quantity", 1, NULL, 'q' },
        { "seed",     1, NULL, 's' },
        { "phrase",   1, NULL, 'p' },
        { "key",      1, NULL, 'k' },
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
            args->output_file_path = optarg;
            break;

        case 'i':
            args->input_file_path = optarg;
            break;
        
        case 'v':
            verbose = 1;
            break;

        case 'q':
            args->qnt = (unsigned int )strtoul(optarg, NULL, 10);
            break;

        case 'p':
            args->secret_phrase = optarg;
            break;

        case 'k':
            args->encrypt_key_file_path = optarg;
            break;

        case 's':
            args->seed = strtoul(optarg, NULL, 10);
            break;

        case '?':
            PrintHelp(stderr, program_name, 1);

        case -1:
            break;

        default: 
            printf("Undefined option '%c'. Ignored.", next_option);
            break;
        }
    }
    while (next_option != -1); 
    if (verbose)
    {
        printf("Parsed arguments:\n");
        printf("Command = %d\n", args->command);
        printf("Input = %s\n", args->input_file_path);
        printf("Output = %s\n", args->output_file_path);        
        printf("Quantity = %lu\n", args->qnt);
        printf("Seed = %u\n", args->seed);
    }
}

int main (int argc, char *argv[]) 
{
    LauncherArguments args;
    ParseArguments(argc, argv, &args);

    // char path_buff[LONG_PATH_BUFF_SIZE] = {0};

    // char* cur_dir_path;
    // if (readlink ("/proc/self/exe", path_buff, LONG_PATH_BUFF_SIZE) != -1)
    // {
    //     cur_dir_path = dirname(path_buff);
    // }

    if (args.command == 0)
    {
        // determine full lib path
        // strcpy(path_buff, cur_dir_path);
        // strcat(path_buff, "/");
        // strcat(path_buff, DATA_CREATOR_LIB_FILE_NAME);

        void*  handle = dlopen(DATA_CREATOR_LIB_FILE_NAME, RTLD_NOW);
        if (handle == NULL)
        {
            printf("Error. System unable to load %s library.", DATA_CREATOR_LIB_FILE_NAME);
            return 1;
        }
        //check handle
        typedef int (* generate_data_func_t) (const char*, const unsigned long, const char*, const unsigned int, const char*);
        generate_data_func_t  generate_data_func = (generate_data_func_t ) dlsym (handle, GEN_DATA_FUNC_NAME);
        if (generate_data_func == NULL)
        {
            printf("Error. %s function of %s library was not load.", GEN_DATA_FUNC_NAME, DATA_CREATOR_LIB_FILE_NAME);
            return 1;
        }
        //check function
        return (*generate_data_func)(args.input_file_path, args.qnt, args.output_file_path, args.seed, args.encrypt_key_file_path);
    }
    else if (args.command == 1)
    {
        printf ("Encryption to be implemented.\n");

        // const char* plain_text = "AAA The Message AAA";
        // printf ("This is plain text: %s\n", plain_text); 

        // char buff[128];
        // memset(buff, 0, sizeof(buff));
        // if (Encrypt(plain_text, args.encrypt_key_file_path, args.secret_phrase, buff, 128))
        // {
        //     printf ("This is encripted text: %s\n", buff);
        //     //Just for initial check proposes
        //     char buff2[128];
        //     memset(buff2, 0, sizeof(buff2));
        //     if (!Decrypt(buff, args.encrypt_key_file_path, args.secret_phrase, buff2, 128))
        //     {
        //         printf ("Decrypt fails!\n");
        //         return 1;
        //     }
        //     printf ("This is decrypted text: %s\n", buff2);
        // }
        // else
        // {
        //     printf ("Encrypt is failed.\n");
        //     return 1;
        // }
    }
    else if (args.command == 2)
        printf ("Decryption to be implemented.\n");
    else
        printf ("Undefined command. Please specify -g, -e or -d.\n");

    return 0;
}

