import React, { Component } from 'react';
import Sensors from './Sensors'
import './index.css';

class Dashboard extends Component {
  constructor(props) {
    super(props);
    this.state = { 
      list: [
        { id: 1,
          values: [
            { name: 'Ketinggian', value: '73cm' },
            { name: 'Suhu', value: '24C' }
        ]},
        { id: 2,
          values: [
            { name: 'Ketinggian', value: '73cm' },
            { name: 'Suhu', value: '24C' },
            { name: 'Tekanan', value: '1 bar'},
            { name: 'Ketinggian', value: '73cm' },
            { name: 'Suhu', value: '24C' },
            { name: 'Tekanan', value: '1 bar'}
        ]}
    ]}
  }
  render() { 
    return ( 
      <div>
        <h2>Dashboard</h2>
        {this.state.list.map(list => 
          <Sensors  
            id={list.id}
            values={list.values}
          />
        )}
      </div>
    );
  }
}
 
export default Dashboard;