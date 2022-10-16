import React from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'

/*
React Landing Page
Author: Sreyansh Mamidi
Date: 10/16/2022
References: https://reactjs.org/tutorial/tutorial.html, W3Schools
Notes:
    - Focused on learning how to use React
    - Fixed and added formatting
    - Added date/time to the page
    - Added the onClick event for each button
To Do:
    - Connect front-end interface to data analysis results
*/
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
            <button 
            className={b_class}
            onClick={this.select.bind(this)}
            >
            {this.props.value}
            </button>
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
        let day_of_week = "";

        if (day === 0) {
            day_of_week = "Sunday";
        }
        if (day === 1) {
            day_of_week = "Monday";
        }
        if (day === 2) {
            day_of_week = "Tuesday";
        }
        if (day === 3) {
            day_of_week = "Wednesday";
        }
        if (day === 4) {
            day_of_week = "Thursday";
        }
        if (day === 5) {
            day_of_week = "Friday";
        }
        if (day === 6) {
            day_of_week = "Saturday";
        }

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