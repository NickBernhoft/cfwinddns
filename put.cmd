:: for some reason i cannot make this a label in main with a "goto"

curl --request PATCH ^
--url "https://api.cloudflare.com/client/v4/zones/%zone_id%/dns_records/%dns_record_id%" ^
--header "Content-Type: application/json" ^
--header "X-Auth-Email: %email%" ^
--header "X-Auth-Key: %api_key%" ^
--data "{""content"": ""%new_ip%""}"