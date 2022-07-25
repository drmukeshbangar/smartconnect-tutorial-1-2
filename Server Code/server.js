/*
 * Copyright 2022 @ DR. MUKESH BANGAR. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

const http = require('http');
const socketio = require('socket.io');
const PORT = 5000;


async function run() {
	const port = process.env.PORT || PORT;
	const server = http.createServer();
	const io = socketio(server);

	server.listen(port, () => {
		console.log(`Running socket on port: ${port}`);
	});
//Data recive here
	io.on('connection', (socket) => {
          socket.on('muks', async (vals) => {
			console.log("vals: " + vals);
//Send data to muksOS for perform functions such as speaking, Set Reminders, Set Alarm
		io.emit('esp', vals);


		});

		socket.on('from_esp', async (vals) => {
			console.log("vals: " + vals);

		io.emit('data', vals);


		});

		});
		

}

run();
