
# Self Driving Car -OneZero

Experience the future of transportation with our Self-Driving Car System, powered by Raspberry Pi and Arduino. This autonomous marvel combines PID control with Canny edge detection and Hough Transform for precise lane navigation.

Addressing the need for safer and more efficient roads, our affordable system minimizes errors. Raspberry Pi and Arduino components make it an ideal platform for education and experimentation.

## Design

Hardware: Raspberry Pi for streaming live footage to the main computer. while Arduino manages low-level control.

Software: Canny Edge Detection and Hough Transform for lane detection, PID controller for steering adjustments.

Workflow: Real-time image capture, streaming to the main computer, edge detection, lane extraction, and dynamic steering adjustments ensure adaptive navigation.











## Demo

https://github.com/Dipin-Adhikari/Self-Driving-Car-OneZero/blob/main/Demo%20Video/demo.mp4

## Installation

``` bash
  git clone https://github.com/Dipin-Adhikari/Self-Driving-Car-OneZero.git
  cd Self-Driving-Car-OneZero
  pip install -r requirements.txt
```
## Run the project

```bash
    RaspberryPi: python main.py
    Computer: python main.py
```

## Authors

- [@Dipin Adhikari](https://github.com/Dipin-Adhikari)
- [@Nishan Kandel](https://github.com/NishantKandel)

