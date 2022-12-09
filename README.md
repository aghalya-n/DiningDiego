# course-project-group-12
course-project-group-12 created by GitHub Classroom

# Functionality

This is a full stack interactive web application that was created primarily using React and Java. It displays crowd levels at different meal times (i.e. Breakfast, Lunch, Dinner) at the four dining halls of the University of Illinois at Urbana-Champaign: Lincoln Avenue Residence Hall, Pennsylvania Avenue Residence Hall, Illinois Street Residence Hall, and Ikenberry Dining Center. It was created with the intention of allowing students to see if they would be attending the Dining Hall during busy hours without having to be there. On the page, it displays each of the dining halls at UIUC, and the user can select and interact with the dining hall of their choice in order to understand the number of individuals entering the Dining Hall at a specific time.

# Inspiration

Our project was inspired based on the Google integrated restaurant activity checker, where, if you search on Google Maps for a restaurant, you would be able to see the restaurant capacity and time. 

There is no software that performs the same job as ours on University of Illinois campus, and what makes our project particularly useful is the idea that we are using data directly from number of swipes at the Dining Hall over the course of six days and filtered by weekday and minute, giving us an accurate estimate on the number of diners entering the hall. The Illinois App also exists, but that only gives you the times and meal options, but not specifically the amount of people at the Dining Hall.

We have all found ourselves going to the Dining Hall at its peak times, and this app was something that was inspired by allowing future students to avoid this issue with the use of our trusted pal, Dining Diego!

# Technical Architecture

Below listed is our diagram:

![image](https://user-images.githubusercontent.com/87607418/206287350-a06432a2-6fc0-4469-b95c-15dc50e023f2.png)

The diagram above is an explanation of how we integrate the spreadsheets of six weeks worth of Dining Hall entry data, and from there, the data was parsed to be formatted in a way interpretable by the Backend. Once in the backend, all data is placed into the MySQL database, and the Backend is then used for interactive components for users in the Frontend, which is used for the website interface.

For each of these components, we delegated specific members to work on the individual tasks.
The front end of this project was built mainly using Javascript's React library in addition to CSS for formatting, fonts, and coloring. The backend was built using Java and Python (specifically the pandas library) to coordinate, organize, and analyze the student crowd data at various times that we received through contacting UIUC housing.

# Using Our Project
1. Clone the repo onto your local machine.
2. Ensure that you have MySQL and MySQL Workbench installed in order to store the data.
3. Open the backend portion of the project in your IDE of choice for java applications.
4. Open the frontend portion of the project in your idea or editor of choice.
5. Install Postman or other alternatives to test the supported APIs. A collection can be found in the repo.
6. Usee npm install in your terminal on the front end portion to obtain all the node modules.
7. Run both the spring boot application and front end application and you are all set.

# Group Member Role Description:

Nathan Joseph: Gathered and organized all of the university dining hall data, as well as completed the information analysis by working with Python, and worked on the README file to explain project to viewers. Met with mentor and groupmates to effectively integrate data and discuss solutions to possible errors.

Sreyansh Mamidi: Worked on the front end of the web application using React and CSS, in addition to integrating the front end with the back end. Met with mentor and groupmates to effectively integrate data and discuss solutions to possible errors.

Siddharth Tangri: Integrated the Python data with Java and furnished the backend using Java to parse the data and output the right values of the crowd levels corresponding to certain times on each day of the week. Met with mentor and groupmates to effectively integrate data and discuss solutions to possible errors. Also helped Sreyansh with the front-end.

Aghalya Narayanan: Researched for the front end of the web application using React, in addition to contibuting to the README file to explain the project to viewers. Met with mentor to participate in discussion.
