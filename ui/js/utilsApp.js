const getElement = (parent, selector) => parent.querySelector(selector);
const getElements = (parent, selector) => parent.querySelectorAll(selector);
const convertSecs = (secs) => `${secs / 60}:${secs % 60}`;
