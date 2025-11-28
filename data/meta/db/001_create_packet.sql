CREATE TABLE packets (
    id SERIAL PRIMARY KEY,
    src_ip TEXT,
    dest_ip TEXT,
    protocol TEXT,
    timestamp TIMESTAMP,
    payload_size INTEGER
	
);