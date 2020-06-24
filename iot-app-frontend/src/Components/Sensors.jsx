import React, { Component } from 'react';
import './index.css';

class Sensors extends Component {
  constructor(props) {
    super(props);
    this.state = {  }
  }
  render() { 
    return ( 
      <div class='sensorBorder'>
        <h3>Sensor {this.props.id}</h3>
        <div class='sensorContainer'>
          {this.props.values.map(value => 
            <div>
              <div class='sensorName'>{value.name}</div>
              <div class='sensorValue'>{value.value}</div>
            </div>
          )}
        </div>
      </div>
    );
  }
}
 
export default Sensors;