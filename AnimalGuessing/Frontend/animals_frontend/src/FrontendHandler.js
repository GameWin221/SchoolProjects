import React from 'react'
import Button from './Button'
import InputField from './InputField'

export default class FrontendHandler extends React.Component {
    //constructor(props) {
    //    super(props)
    //}

    sendAnswer(operation, text) {
        alert('Sending answer: ' + text);

        this.props.frontendApi.sendStr(operation, text, (expectation, response) => {
            console.log('[' + expectation + ']: "' + response + '"')
        });
    }

    render() {
        return(
            <div className='main'>
                <Button caption="Start" onClick={()=>this.sendAnswer("start", "start")}/>

                <div className='buttons'>
                    <Button caption="Yes" onClick={()=>this.sendAnswer("answer", "yes")}/>
                    <Button caption="No" onClick={()=>this.sendAnswer("answer", "no")}/>
                </div>

                <InputField frontendApi={this.props.frontendApi} label="Animal" operation="animal"/>
                <InputField frontendApi={this.props.frontendApi} label="Question" operation="question"/>
                
            </div>
        )
    }
}