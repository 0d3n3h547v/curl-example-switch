/*...........Dontwait00..Project...........*/

/* include */
#include <switch.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
/* define */
#define EXIT_SUCCESS 0

// this "write_callback" probably is used for writing into FILE *destinaton
size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(NULL);
	printf("press A to continue or press + to exit\n");
	
	CURL *curl; 
	FILE *fp;
	CURLcode res;
	char *url = "http://www.wiiubru.com/appstore.nro"; // string used for curl_easy_setopt
	char outfilename[FILENAME_MAX] = "sdmc:/switch/appstoreNXtest.nro"; // file used for curl_easy_setopt
	curl_global_init(CURL_GLOBAL_DEFAULT); // initializing/calling functions
	curl = curl_easy_init(); // initializing/calling functions
	
    // Main loop
    while(appletMainLoop())
    {
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO); // can do even: /* if( hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS) {//...} */ but is not very efficient.

        if (kDown & KEY_PLUS) goto EXIT; // terminate the program.
		
		if (kDown & KEY_A) break; // break loop
		
		gfxFlushBuffers();
        gfxSwapBuffers();
	}
	
	consoleClear();
	printf("starting libcurl...\n");
    
    if (curl)
	{
		printf("\n\nURL = %s\nFile = %s\n\nDownload...\n", url, outfilename);
        fp = fopen(outfilename,"wb");printf("\n*DEBUG*\n");
        curl_easy_setopt(curl, CURLOPT_URL, url); // getting URL from char *url
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // debugging
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // following HTTP redirects
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L); // a max of timeout setopt for 20s
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skipping cert. verification, if needed (ACTUALLY IS NEEDED FROM LIBNX)
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skipping hostname verification, if needed (ACTUALLY IS NEEDED FROM LIBNX)
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // writes data into pointer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); // writes pointer into FILE *destination
        res = curl_easy_perform(curl); // perform tasks curl_easy_setopt asked before
		if (res != CURLE_OK) {
			printf("\nfailed: %s\n", curl_easy_strerror(res));
		}
		
	curl_easy_cleanup(curl); // always cleanup
    fclose(fp); // closing FILE *fp
	
	}
	
	printf("\ndone! Press B or + to exit");
				
	while(appletMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		if ((kDown & KEY_B) || (kDown & KEY_PLUS)) break; /* a simple trick to use both or more keys for the same function as of this example, breaking while loop */
		
		gfxFlushBuffers();
        gfxSwapBuffers();
	}
	
	EXIT: //spaghetti code :(
    gfxExit();
	curl_global_cleanup();
    return EXIT_SUCCESS;
}

