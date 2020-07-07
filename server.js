const express = require('express');
const app = express();
const port = 4000;

const MongoClient = require('mongodb').MongoClient;
const url = "mongodb://localhost:27017/";

let cors = require('cors');
const { ObjectID, ObjectId } = require('mongodb');
app.use(cors());

let currentValue1 = 0;
let currentValue2 = 0;
let object;

app.get('/api', function(req, res) {
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("iot-app-db");
    dbo.collection("sensors").find({}).toArray(function(err, result) {
      if (err) throw err;
      res.status(200).json(result);
      db.close();
    });
  }); 
})

app.post('/api', async function(req, res) {
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("iot-app-db");
    if(ObjectId.isValid(req.query.id)) {
      var myquery = { _id: new ObjectId(req.query.id) };
      var newvalues = { $set : {'sensors.0.value' : req.query.var1} };
      dbo.collection("sensors").updateOne(myquery, newvalues, function(err, res) {
        if (err) throw err;
      });
      var newvalues = { $set : {'sensors.1.value' : req.query.var2} };
      dbo.collection("sensors").updateOne(myquery, newvalues, function(err, res) {
        if (err) throw err;
        db.close();
      });
      console.log("1 document updated");
    }
    else {
      console.log("Warning: ObjectId not valid");
    }
  });
})

app.listen(port, () => console.log(`Example app listening at http://localhost:${port}`))