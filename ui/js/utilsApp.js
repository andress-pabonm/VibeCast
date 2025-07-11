const getElement = (parent, selector) => parent.querySelector(selector);
const getElements = (parent, selector) => parent.querySelectorAll(selector);
const convertSecs = (secs) => {
    const minutes = Math.floor(secs / 60);
    const seconds = Math.floor(secs % 60);
    return `${minutes}:${seconds < 10 ? '0' : ''}${seconds}`;
};