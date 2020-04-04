const express = require('express'),
    bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.urlencoded({
    extended: true
}));

// Mock Up Local Data
var data = [{
    id: 1,
    type: 'gateway',
}, {
    id: 2,
    type: 'repeater'
}, {
    id: 3,
    type: 'repeater'
}, {
    id: 10,
    type: 'sensor',
    status: {
        car: true,
        batt: 4
    }

}, {
    id: 11,
    type: 'sensor',
    car: true,
    batt: 3
}, {
    id: 12,
    type: 'sensor',
    car: false,
    batt: 4
}, {
    id: 13,
    type: 'sensor',
    car: false,
    batt: 2
}];


// Testing Route
app.get('/', (req, res) => {
    res.send("Hey there! welcome!");
});

//======Gateway Handler
//===========================

// GET - ALL data 
app.get('/api/gateway/:location/all', (req, res) => {
    res.json(data);
});

// POST - ALL data
app.post('/api/gateway/:location/all', (req, res) => {
    res.send("okay thanks!");
    console.log(req.body);

    data.push(req.body);
    console.log(data);
});

// POST - ROUTINE update data
app.post('/api/gateway/:location/routine', (req, res) => {
    res.send("thanks for the update!");
    console.log(req.body);

    objIndex = data.findIndex((obj => obj.id == req.body.id));
    data[objIndex].status = req.body.status;
    console.log(data[objIndex]);
});


app.listen(port, () => {
    console.log(`Hi! Server is listening on port ${port}`);
});