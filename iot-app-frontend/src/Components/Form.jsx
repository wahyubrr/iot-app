import React, { Component } from 'react';

class Form extends Component {
  constructor(props) {
    super(props);
    this.state = { 
      sensorCount: 1
    }
  }

  sensorTable() {
    return <h3>{this.state.sensorCount}</h3>
  }

  render() { 
    return ( 
      <form onSubmit={this.handleSubmit}>
        <label>Sensor name:</label><br/>
          <input type="text" value={this.state.value} onChange={this.handleChange} />
        <button onClick={() => this.setState({sensorCount: this.state.sensorCount+1})}>Add sensor</button>
        <button onClick={() => this.setState({sensorCount: this.state.sensorCount-1})}>Decrease sensor</button>
        {this.sensorTable()}
        <input type="submit" value="Submit" />
      </form>
    );
  }
}
 
export default Form;