import React, { Component } from 'react';
import './index.css';

class Sensors extends Component {
  constructor(props) {
    super(props);
    this.state = {  }
  }
  render() { 
    return ( 
      <div className='sensorBorder'>
        <h3>{this.props.name}</h3>
        <p>Sensor ID: {this.props.keys}</p>
        <div className='sensorContainer'>
          {this.props.sensors.map((value, index) => 
            <div key={index}>
              <div className='sensorName'>{value.sensorName}</div>
              <div className='sensorValue'>{value.value} {value.unit}</div>
            </div>
          )}
        </div>
      </div>
    );
  }
}
 
export default Sensors;