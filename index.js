import React from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'
import {Button, AppBar, Container, Toolbar, Typography, ButtonGroup, Avatar} from '@mui/material';

/*
React Landing Page
Author: Sreyansh Mamidi
Date: 11/20/2022
References: Material UI (https://mui.com/), W3Schools
*/

// Illini Orange: #FF5F05, Illini Blue: #13294B
// https://marketing.illinois.edu/visual-identity/color
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
            style = {{backgroundColor: '#13294B', maxWidth: '150px', maxHeight: '100px', minWidth: '150px', minHeight: '100px'}}
            className={b_class}
            onClick={this.select.bind(this)}
            >
            <span style={{fontSize: '20px'}}> {this.props.value} </span>
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
        const intro_message = "Select a dining hall to get started.";
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
         * Note: JSX expressions must have 1 parent element -> can't return multiple values
         */
        return (
            <>
                {/* Creates the navigation bar, displays the current date and time */}
                <AppBar position="static" style={{background: '#13294B'}}>
                    <Container maxWidth="x1">
                        <Toolbar disableGutters>
                            <Typography variant="h6" sx={{fontFamily: 'monospace', fontWeight: 500}}> <strong> Dining Diego at the University of Illinois </strong>  — </Typography>
                            &nbsp;
                            &nbsp;
                            <Typography variant="h6" sx={{fontFamily: 'monospace', fontWeight: 500}}> {day_of_week}   {date}           {time} </Typography>
                            &nbsp;
                            &nbsp;
                            &nbsp;
                            <Avatar alt="UIUC" src="https://marketing.illinois.edu/wp-content/uploads/2021/09/block-I-orange-background.png" sx={{width: 40, height: 40}}/>
                        </Toolbar>
                    </Container>
                </AppBar>
                {/* Provides insructions about how to use the web app */}
                <div>
	                 <h2 class="main-head" style={{color: '#FF5F05', fontFamily: 'monospace'}}> Welcome </h2>
	                 <div className='message' style={{fontFamily: 'sans-serif'}}> {intro_message} </div>
	                 <div className='page'>
	                 <br></br>
                     <h2 class="main-head" style={{color: '#FF5F05', fontFamily: 'monospace'}}> Dining Halls </h2>
                     <div className='message' style={{fontFamily: 'sans-serif'}}> The University of Illinois has <strong> four </strong> buffet-style dining options. </div>
	                 <br></br>
                     {/* Displays the dining halls that the user can choose from */}
                     <ButtonGroup variant="outlined" size="large">
                        {this.renderDiningHall("ISR")}
                        &nbsp;
                        {this.renderDiningHall("IKE")}
                        &nbsp;
                        {this.renderDiningHall("PAR")}
                        &nbsp;
                        {this.renderDiningHall("LAR")} 
                     </ButtonGroup>
	                </div>
                </div>
            </>
        );
    }
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<Page />);