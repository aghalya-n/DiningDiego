import React from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'

/*
Basic React Landing Page
Author: Sreyansh Mamidi
Date: 10/7/2022
References: https://reactjs.org/tutorial/tutorial.html
Notes:
     - Focused on learning how to use React
     - Will continue to add more features to this template
*/
class Dining extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
        <div>
            <button 
            className="dining"
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
        return (
            <div>
                <h2> Dining Diego at UIUC </h2>
                <div className='message'> {message} </div>
                <div className='page'>
                {this.renderDiningHall("ISR")}
                {this.renderDiningHall("IKE")}
                {this.renderDiningHall("PAR")}
                {this.renderDiningHall("LAR")} 
                </div>
            </div>
        );
    }
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<Page />);