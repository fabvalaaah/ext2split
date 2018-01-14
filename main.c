/*
 * MIT License
 * 
 * Copyright (c) 2018 Fabvalaaah - fabvalaaah@laposte.net
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * DONATION:
 * As I share these sources for commercial use too, maybe you could consider
 * sending me a reward (even a tiny one) to my Ethereum wallet at the address
 * 0x1fEaa1E88203cc13ffE9BAe434385350bBf10868
 * If so, I would be forever grateful to you and motivated to keep up the good
 * work for sure :oD Thanks in advance !
 * 
 * FEEDBACK:
 * You like my work? It helps you? You plan to use/reuse/transform it? You have
 * suggestions or questions about it? Just want to say "hi"? Let me know your
 * feedbacks by mail to the address fabvalaaah@laposte.net
 * 
 * DISCLAIMER:
 * I am not responsible in any way of any consequence of the usage
 * of this piece of software. You are warned, use it at your own risks.
 */

/* 
 * File:   main.c
 * Author: Fabvalaaah
 *
 * 01/14/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#define BOOT_BLOCK_SIZE 1024
#define OUTPUT_FILE_NAME_BUFFER_LENGTH 512
#define EMPTY_BLOCKS_REPORT_NAME "empty_blocks_report.txt"
#define BLOCKS_DIR_PATH "./blocks" 

static const char* BLOCKS_PATH_MASK = "./blocks/%d.blk";

bool isEmptyBlock(unsigned char* block, unsigned long size) {
    unsigned long i;

    if (!block) {
        return true;
    }

    for (i = 0; i < size; i++) {
        if (block[i]) {
            return false;
        }
    }

    return true;
}

int blockExtraction(FILE* fpIn, FILE* fpReport, char* outputFileName,
        unsigned long blockSize, unsigned long* emptyBlockCount) {
    unsigned char* buffer = NULL;
    unsigned long bytesCount = 0;
    FILE* fpOut = NULL;
    int exitCode = EXIT_SUCCESS;

    if (!fpIn || !fpReport || !outputFileName || !blockSize
            || !emptyBlockCount) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_BLOCK_EXTRACTION;
    }

    buffer = malloc((sizeof (unsigned char)) * blockSize);
    if (!buffer) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_BLOCK_EXTRACTION;
    }

    bytesCount = fread(buffer, blockSize, 1, fpIn);
    if (!bytesCount) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_BLOCK_EXTRACTION;
    }

    if (isEmptyBlock(buffer, blockSize)) {
        (*emptyBlockCount)++;
        fputs(outputFileName, fpReport);
        fputs("\n", fpReport);
    }
    fpOut = fopen(outputFileName, "wb");
    if (!fpOut) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_BLOCK_EXTRACTION;
    }
    bytesCount = fwrite(buffer, blockSize, 1, fpOut);
    if (!bytesCount) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_BLOCK_EXTRACTION;
    }

CLEAN_BLOCK_EXTRACTION:
    if (buffer) {
        free(buffer);
    }
    if (fpOut) {
        fclose(fpOut);
    }
    return exitCode;
}

int main(int argc, char** argv) {
    unsigned long blockSize;
    unsigned long blockNum = 0;
    unsigned long emptyBlockCount = 0;
    FILE* fpIn = NULL;
    FILE* fpReport = NULL;
    char outputFileName[OUTPUT_FILE_NAME_BUFFER_LENGTH];
    int exitCode = EXIT_SUCCESS;

    /* Manage command parameters */
    if (3 != argc) {
        printf("Usage: ext2split <block size> <file-system file path>\n");
        exitCode = EXIT_FAILURE;
        goto CLEAN_MAIN;
    }
    blockSize = (unsigned int) atol(argv[1]);
    if (!blockSize) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_MAIN;
    }
    /* ------- */

    /* Open IO files handlers */
    fpIn = fopen(argv[2], "rb");
    if (!fpIn) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_MAIN;
    }
    fpReport = fopen(EMPTY_BLOCKS_REPORT_NAME, "w");
    if (!fpReport) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_MAIN;
    }
    /* ------- */

    /* Create the destination directory */
    mkdir(BLOCKS_DIR_PATH, S_IRWXU | S_IRWXG | S_IRWXO);
    /* ------- */

    /* Extract the boot block */
    memset(outputFileName, '\0', OUTPUT_FILE_NAME_BUFFER_LENGTH);
    sprintf(outputFileName, BLOCKS_PATH_MASK, blockNum);
    printf("Starting block extraction of %s\n", argv[2]);
    if (EXIT_FAILURE == blockExtraction(fpIn, fpReport, outputFileName,
            BOOT_BLOCK_SIZE, &emptyBlockCount)) {
        exitCode = EXIT_FAILURE;
        goto CLEAN_MAIN;
    }
    printf("Boot block successfully extracted to %s\n", outputFileName);
    blockNum++;
    /* ------- */

    /* Extract the other following blocks */
    printf("Now extracting the other blocks, this could take a while...\n");
    sprintf(outputFileName, BLOCKS_PATH_MASK, blockNum);
    while (EXIT_SUCCESS == blockExtraction(fpIn, fpReport, outputFileName,
            blockSize, &emptyBlockCount)) {
        blockNum++;
        sprintf(outputFileName, BLOCKS_PATH_MASK, blockNum);
    }
    printf("Extraction ended successfully (%lu empty blocks detected)\n",
            emptyBlockCount);
    /* ------- */

CLEAN_MAIN:
    if (fpIn) {
        fclose(fpIn);
    }
    if (fpReport) {
        fclose(fpReport);
    }
    return (exitCode);
}
