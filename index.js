import React from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'
import {Button, AppBar, Container, Toolbar, Typography, Box, IconButton} from '@mui/material';
import {MenuIcon} from "@mui/icons-material"

/*
React Landing Page
Author: Sreyansh Mamidi
Date: 11/13/2022
References: https://reactjs.org/tutorial/tutorial.html, W3Schools
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

        /**
         * Connect this interface to back-end data
         * One option: Plotly API - https://plotly.com/javascript/react/
         * JSX expressions must have 1 parent element -> can't return multiple values
         */
        return (
            // <div>
            //     <h2 class="main-head"> Dining Diego at UIUC </h2>
            //     <div className='message'> {message} </div>
            //     <div className='page'>
            //     <br></br>
            //     <b className="time"> Date: {day_of_week}   {date}            Time: {time}</b>
            //     <br></br>
            //     <br></br>
            //     {this.renderDiningHall("ISR")}
            //     <br></br>
            //     {this.renderDiningHall("IKE")}
            //     <br></br>
            //     {this.renderDiningHall("PAR")}
            //     <br></br>
            //     {this.renderDiningHall("LAR")} 
            //     </div>
            // </div>
            <>
                <AppBar position="static">
                    <Container maxWidth="x1">
                        <Toolbar disableGutters>
                            <Typography variant="h6" sx={{fontFamily: 'monospace', fontWeight: 500}}> Dining Diego </Typography>
                                <IconButton
                                    size="large"
                                    edge="start"
                                    color="inherit"
                                    aria-label="menu"
                                    sx={{ mr: 2 }}
                                >
                            </IconButton>
                            {/* {this.renderDiningHall("ISR")} */}
                            <Button color="inherit"> ISR </Button>
                            <Button color="inherit"> IKE </Button>
                            <Button color="inherit"> LAR </Button>
                            <Button color="inherit"> PAR </Button>
                        </Toolbar>
                    </Container>
                </AppBar>
                <br></br>
                <span> Welcome! Select a dining hall to get started. </span>
                <br></br><br></br> <br></br>
                <b className="time"> Date: {day_of_week}   {date}            Time: {time}</b>
            </>

        );
    }
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<Page />);