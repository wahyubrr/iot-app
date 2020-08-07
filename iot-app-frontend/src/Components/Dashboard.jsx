import React, { Component } from 'react';
import Sensors from './Sensors';
import axios from 'axios';
import './index.css';

class Dashboard extends Component {
  constructor(props) {
    super(props);
    this.state = { 
      sensors: [],
      user: 'user1',
      showForm: false,
      sensorid: '',
      value1: '',
      value2: ''
    };
  }

  async componentDidMount() {
    try {
      setInterval(async () => {
        axios.get('http://192.168.1.9:4000/api')
        .then(response => {
          this.setState({sensors: response.data})
        })
        .catch(error => {
          console.log(error)
        })
      }, 1000);
    } catch(e) {
      console.log(e);
    }
  }

  handleClick() {
    this.setState({ showForm: !this.state.showForm });
  }

  handleChange = event => {
    this.setState({ [event.target.name]: event.target.value });
  }

  handleSubmit = event => {
    event.preventDefault();

    const user = {
      sensorid: this.state.sensorid,
      value1: this.state.value1,
      value2: this.state.value2,
      
    };

    if ( parseInt(this.state.value1) > parseInt(this.state.value2) ) {
      axios.post('http://192.168.1.9:4000/api/submit-form-batas', user)
      .then(res => {
        console.log(res);
        console.log(res.data);
      })
    }
    else {
      alert("ALERT! Batas atas harus lebih besar daripada batas bawah!");
    }
  }

  render() { 
    const { sensorid, value1, value2 } = this.state
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
        { this.state.showForm && 
          <div>
            <form onSubmit={this.handleSubmit}>
              <label>Sensor ID:</label><br/>
              <input value={sensorid} name="sensorid" type="text" onChange={this.handleChange} /> <br/>

              <label>Batas Atas:</label><br/>
              <input value={value1} name="value1" type="text" onChange={this.handleChange} /> <br/>

              <label>Batas Bawah:</label><br/>
              <input value={value2} name="value2" type="text" onChange={this.handleChange} /> <br/>

              <button>Submit</button>
            </form>
          </div>
        }
        <button className='buttonMargin' onClick={() => this.handleClick()}>Pengaturan pompa</button>
      </div>
    );
  }
}

export default Dashboard;