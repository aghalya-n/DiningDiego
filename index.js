import React from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'
import Button from '@mui/material/Button';

/*
React Landing Page
Author: Sreyansh Mamidi
Date: 10/16/2022
References: https://reactjs.org/tutorial/tutorial.html, W3Schools
Notes:
    - Researched different CSS frameworks - Material UI (MUI) and Bootstrap
    - Found a version of Plotly's API that's compatible with React (React Plotly.js)
    - Added MUI buttons to the app
To Do:
    - Add more MUI components and implement Plotly.js
*/

// Material UI (https://mui.com/), Bootstrap (https://getbootstrap.com/docs/5.2/getting-started/introduction/)
class Dining extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            blue: true
        }
    }
    select() {
        // Changes the state
        this.setState({blue: !this.state.blue});
    }

    render() {
        // Assigns the button's class through a conditional
        let b_class = this.state.blue ? "dining" : "dining_new";
        return (
        <div>
            <Button
            variant="contained"
            size="large"
            color="success" 
            className={b_class}
            onClick={this.select.bind(this)}
            >
            {this.props.value}
            </Button>
        </div>

        );
    }
}

class Page extends React.Component {
    renderDiningHall(name) {
        return <Dining value={name}/>
    }

    render() {
        const message = "Welcome! Here are the dining options offered at UIUC.";
        var now = new Date();
        var date = now.toLocaleDateString();
        var time = now.toLocaleTimeString();
        var day = now.getDay();

        const day_arr = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
        const days_of_week = new Map();

        for (let i = 0; i < day_arr.length; ++i) {
            days_of_week.set(i, day_arr[i]);
        }

        let day_of_week = days_of_week.get(day);

        // Connect this interface to back-end data
        // One option: Plotly API - https://plotly.com/javascript/react/
        return (
            <div>
                <h2 class="main-head"> Dining Diego at UIUC </h2>
                <div className='message'> {message} </div>
                <div className='page'>
                <br></br>
                <b className="time"> Date: {day_of_week}   {date}            Time: {time}</b>
                <br></br>
                <br></br>
                {this.renderDiningHall("ISR")}
                <br></br>
                {this.renderDiningHall("IKE")}
                <br></br>
                {this.renderDiningHall("PAR")}
                <br></br>
                {this.renderDiningHall("LAR")} 
                </div>
            </div>
        );
    }
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<Page />);