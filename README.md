# cfwinddns
CloudFlare Windows DDNS

## what does this do?
this program automaticly keeps the IP of a given domain up to date with the public IP of whatever computer you're running it on. This is useful for home server hosting because an ISP will usually change your public IP every couple weeks or so.

## How To Use:
### enter these at the top of `main.cmd` (right click -> edit) the descriptions below tell you how to find them this is the only required step

user defined variables:

email

api_key

api_token

zone_id

target_domain

*they should be entered with no extra spaces, the following is an example of what the top of main.cmd should look like*
```batch
set email=myemailaddress@email.com
set api_key=8118327118133ac35e32b6f294c41ada4be7d
set api_token=0d0c1441a37dbc7c27be5d6baf79dcedc7abc
set zone_id=545e73b389c3d4e3f44831429b123db4
set target_domain=minecraft.bigfrogs.win
```

variables fetched by the program:
dns_record_id
new_ip

## Running
This program comes ready to go. You can just enter the variables at the top of main.cmd, and when youre done run it. if its not working you will know.

if you have GCC installed, you can delete `cmdgen.exe`, main.cmd will detect the absence of it and recompile the included C sourcecode using GCC.


## How To Find User Defined Variables Variables

# zone_id:
this is the identifier for an entire domain. Cloudflare Dashboard -> websites -> [your domain] -> Overview -> Zone ID (shown on the page)

# email
this is the email your cloudflare account is made with


# api_token
This is an API tokan generated that has access to the site you would like to have the ddns apply to. You can generate one

My Profile -> API Tokens -> API Tokens (Create Token) -> Edit zone DNS (use template)

instructions for Create Token:
Permissions: Zone | DNS Settings | Edit
Zone Resources: Include | Specific Zone | [Name of your domain]
Client IP Address Filtering: [you may leave this blank or set it if you like]
TTL: [you may leave this blank or set it if you like]

-> continue to summary -> create Token


# api_key
this is your global API key. this can be found on your cloudflare dashboard on the site under "My Profile" >> API Tokens >> GlobalAPI key

# wait
This is the checking interval in seconds, default setting is one hour. The program is pretty lightweight so lowering this did not greatly increase power or CPU usage for me.


# dns_record_id:
this will be fetched by the program


# new_ip
this is fetched by the program



## info about this program / not important for using it

This is my first open source programming project!

This program was made to run natively in windows with only batch / cmd scripts and one executable. Compiling it yourself is optional.

I made this as a project for myself because i did not want to sign up and or pay for a DDNS service, and there were no existing programs i could find that did what this one does for me on windows. This program is for people who wanna see a problem they have, look at the git hub page that everone tolf them to, and not have to know what a compiler is or how to install GCC to the system PATH. I mean that is the type of person running a file server or minecraft server from an old windows PC anyways, right?


# Compiling?
Yes it comes with an exe. You can just enter the data from cloud flare and leave it running the background.
if you like you can compile it yourself. To do this delete `cmdgen.exe`
and main.cmd will detect its absense and use GCC to recompile it.

# Mechanics
the scripts job is to run Cloudflare API calls and an IP checking command
and save the output text files that the C program uses.

The C program parses the data we need from the text files.
and generate a new .cmd file that is then called by main.cmd

There is some good room for expandability with the C program, right now it creates a struct with a profile for every subdomain returned from the `GET` API call. Its very neat and easy to access info for any given domain. This could be used to add more DNS editing functionalty, or recycled to be used to automate other domain managment tasks that are accessable through through the APIthat i dont even know about.
