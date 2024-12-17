// supports up to:
// 1024 subdomain records
// 8 million chatacters api call output
// file names up to 256 characters
// Domain name length up to 1024 characters

// TODO
// use system filesize calls to allocate / get file contents strings
// make datastructure for domain data dynamically allocated??
// move the struct array creation to its own init function?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define NAMELEN 256
#define DOMAIN 1024
#define BLOCKSIZE 1024 * 1024 * 8 // yes its 8 megabytes.. just in case

//contains the needed information about a subdomain
struct sdinfo
{
	char *dns_record_id;
	char *subdomain_name;
	char *ip;
};

int printRecords(struct sdinfo *records);
struct sdinfo *findDomain(struct sdinfo *records, char* name);


// takes in CMDinfo_fd.txt
int main(int argc, char const *argv[])
{
	char *currentIP;
	char dns_file_name[NAMELEN];
	char targetDomain[DOMAIN];
	char ip_file_name[NAMELEN];
	struct sdinfo records[DOMAIN];

	printf("argc: %d\n", argc);
	printf("argv[1]: %s\n", argv[1]);
	printf("argc[2]: %s\n", argv[2]);
	printf("argc[3]: %s\n", argv[3]);



	// check for unexpected input
	if(argc == 1)
	{
		printf("missing arguments\n");
		printf("format: CMDinfo_fd [dns_file_name] [SUBDOMAIN_NAME]\n");
		return 0;
	}	

	if(strlen(argv[1]) > NAMELEN)
	{
		printf("File name greater than max length (%d) for %s\n", NAMELEN, argv[1]);
		return -1;
		//argv[2] = realloc(argv, sizeof(char) * NAMELEN);
	}

	if(argc >= 3 && strlen(argv[2]) > DOMAIN)
	{
		printf("Subdomain name greater than max length (%d) for %s\n", DOMAIN, argv[2]);
		return -2;
	}

	if(argc >= 4 && strlen(argv[3]) > NAMELEN)
	{
		printf("File Name length greater than max length (%d) for %s\n", NAMELEN, argv[3]);
		return -3;
	}

	strcpy(dns_file_name, argv[1]);
	strcpy(targetDomain, argv[2]);
	strcpy(ip_file_name, argv[3]);

	printf("dns_file_name: %s\n", dns_file_name);
	printf("targetDomain: %s\n", targetDomain);
	printf("ip_file_name: %s\n", ip_file_name);

	// file descriptor
	int CMDinfo_fd = open(dns_file_name, O_RDONLY);
	int IPinfo_fd = open(ip_file_name, O_RDONLY);

	printf("CMD info fd: %d\n", CMDinfo_fd);
	printf("CMD info fd: %d\n", IPinfo_fd);

	char *dns_info_string = malloc(sizeof(char) * BLOCKSIZE);
	char *ip_info_string = malloc(sizeof(char) * BLOCKSIZE);

	// if your domain diagnostic output is over 8 megabytes,
	// you should not be using this tool. seek help.
	if(read(CMDinfo_fd, dns_info_string, BLOCKSIZE) == -1)
	{
		printf("DNS info file not found: %s\n", dns_file_name);
	}

	if(read(IPinfo_fd, ip_info_string, BLOCKSIZE) == -1)
	{
		printf("IP info file not found: %s\n", ip_file_name);
	}



	// note that the contents of the structs is dependent on
	// protecting the data in dns_info_string
	printf("\n\ndns_info_string:\n\n%s", dns_info_string);
	printf("\n\n\nip_info_string:\n\n%s\n", ip_info_string);


	int count = 0;
	char *temp;


	// parse from DNSinfo.txt
	strtok(dns_info_string, "\"");
	while( (temp = strtok(NULL, "\"")) )
	{

		// evalusted top to bottom on the doc. order is important
		if(!strcmp(temp, "id"))
		{
			// flush the ":"
			strtok(NULL, "\"");

			records[count].dns_record_id = strtok(NULL, "\"");
		}

		if(!strcmp(temp, "name"))
		{
			// flush the ":"
			strtok(NULL, "\"");

			records[count].subdomain_name = strtok(NULL, "\"");
		}


		if(!strcmp(temp, "content"))
		{
			// flush the ":"
			strtok(NULL, "\"");

			records[count].ip = strtok(NULL, "\"");

			// INCREMENET ON THE DATA THAT APPEARS
			// LOWEST ON THE LIST FOR EACH SITE
			count++;
		}
	}

	// tokenize current public ip from IPinfo.txt
	strtok(ip_info_string, " ");
	while( (temp = strtok(NULL, " \n")) )
	{
		printf("--%s--\n", temp);
		// evalusted top to bottom on the doc. order is important
		if(!strcmp(temp, "Name:"))
		{
			while(temp = strtok(NULL, " \n") )
			{
				if(!strcmp(temp, "Address:"))
				{
					// flush the spaces in the doc
					//strtok(NULL, " ");
					currentIP = strtok(NULL, " \n");
					printf("currentIP: %s\n\n", currentIP);
				}
			}

			records[count].dns_record_id = strtok(NULL, "\"");
		}
	}
	// note the data in dns_info_string and ip_info_string
	// are now damaged. refer to records for data.

	

	printRecords(records);

	struct sdinfo *edit = findDomain(records, targetDomain);

	printf("editVar: %s\n", edit->subdomain_name);



	// generate cmd.out
	// if current ip is different than ip on record
	FILE *output;
	if( (output = fopen("cmdout.cmd", "w")) == NULL)
	{
		printf("Error opening or creating 'cmdout.cmd'\n");
		return -5;
	}



	if(strcmp(currentIP, edit->ip))
	{


		fputs("set dns_record_id=", output);
		fputs(edit->dns_record_id , output);
		fputc('\n', output);

		fputs("set new_ip=", output);
		fputs(currentIP , output);
		fputc('\n', output);

		fputs("call put\n", output);
		fputs("echo  \n", output);
		fputs("echo  \n", output);
		fputs("echo updating IP for %target_domain% to: %new_ip%", output);

	}
	else
	{
		fputs("echo IPs are in Sync!\n", output);
	}




	fclose(output);
	close(CMDinfo_fd);
	close(IPinfo_fd);
	free(ip_info_string);
	free(dns_info_string);
	return 0;
}



// returns number entries printed
int printRecords(struct sdinfo *records)
{
	int i;
	for(i = 0; records[i].dns_record_id; i++)
	{
		printf("records[%d]\n", i);
		printf("Name: %s\n", records[i].subdomain_name);
		printf("IP: %s\n", records[i].ip);
		printf("DNS record ID%s\n\n", records[i].dns_record_id);
	}
	return i;
}

struct sdinfo *findDomain(struct sdinfo *records, char* name)
{	
	for(int i = 0; records[i].subdomain_name; i++)
	{
		if( !strcmp(records[i].subdomain_name, name) )
		{
			return &records[i];
		}
	}

	printf("error: could not find Domain Name: [%s]\n", name);
	return 0;
}