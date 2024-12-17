set email=
set api_key=
set api_token=
set zone_id=
set target_domain=
:: checking interval (in seconds)
set wait=3600

:: comment this out if you only want to see all the commands run
@echo off

title cfwinddns (%target_domain%)

if NOT exist cmdgen.exe (echo building)
if NOT exist cmdgen.exe (gcc cmdgen.c -o cmdgen)

:: fetch data so we can find the dns record id
:start
echo fetching dns info
curl -X GET ^
--url "https://api.cloudflare.com/client/v4/zones/%zone_id%/dns_records" ^
-H "Authorization: Bearer %api_token%" ^
-H "Content-Type: application/json" ^
--header "X-Auth-Email: %email%" ^
--header "X-Auth-Key: %api_key%" -silent > DNSinfo.txt

:: fetch the data for your current public IP
echo fetching ip info
nslookup myip.opendns.com resolver1.opendns.com > IPinfo.txt

:: 
echo generating command output
start /wait cmdgen DNSinfo.txt %target_domain% IPinfo.txt

:: if the the domain IP is the same as your ip, the program will do nothing
:: this call will set the "dns_record_id" and "new_ip" variables
:: and goto PUT
call cmdout.cmd

:mids
echo %date% %time%
call clean
timeout %wait%

goto start


:exit
exit