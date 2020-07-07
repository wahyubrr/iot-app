import React, { Component } from 'react';
import Sensors from './Sensors';
import Form from './Form';
import axios from 'axios';
import './index.css';

class Dashboard extends Component {
  constructor(props) {
    super(props);
    this.state = { 
      sensors: [],
      user: 'user1',
      showForm: false
    };
  }

  componentDidMount() {
    axios.get('http://localhost:4000/api')
      .then(response => {
        this.setState({sensors: response.data})
      })
      .catch(error => {
        console.log(error)
      })
  }

  handleClick() {
    this.setState({ showForm: !this.state.showForm });
  }

  render() { 
    return ( 
      <div>
        <h2>Dashboard</h2>
        {this.state.sensors.map((sensors, index) => 
          <Sensors  
            key={index}
            keys={sensors._id}
            name={sensors.sensorName}
            sensors={sensors.sensors}
          />
        )}
        {this.state.showForm && <Form/>}
        <button className='buttonMargin' onClick={() => this.handleClick()}>Add Sensor</button>
      </div>
    );
  }
}

export default Dashboard;