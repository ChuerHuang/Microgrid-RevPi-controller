%% connect to the TCP server (C++) in VS code
% Connect to the TCP server
tcp_client = tcpclient("192.168.137.2", 8080);  % Use 'localhost' if both client and server are on the same machine

% Send a message to the server
% write(tcp_client, uint8('Hello from MATLAB'));

% Send two float values
data_to_send = typecast(single([12.3, 4.5]), 'uint8');  % Convert floats to bytes
write(tcp_client, data_to_send);


% % Read the response from the server
% pause(1);  % Wait a moment for the server to respond
% data = read(tcp_client, tcp_client.NumBytesAvailable);
% disp(char(data'));

% Read raw binary data
pause(1);
data = read(tcp_client, tcp_client.NumBytesAvailable);
% Convert the binary data into single-precision floats
float_data = typecast(uint8(data), 'single');
% Display the received floating-point values
disp('Received data:');
disp(float_data);
disp(['float_data data type: ', class(float_data)]);


% %% connect to the TCP server (Python) in RevPi
% % Replace 'RevPi_IP' with the actual IP address of your RevPi
% tcp_client = tcpclient('169.254.221.103', 8080);
% 
% % Send a message to the server
% write(tcp_client, uint8('Hello from MATLAB'));
% 
% % Read the response from the server
% pause(1);  % Wait a moment for the server to respond
% data = read(tcp_client, tcp_client.NumBytesAvailable);
% disp(char(data'));

