import React from 'react'
import Button from './Button'
import InputField from './InputField'

export default class FrontendHandler extends React.Component {
    constructor(props) {
        super(props)
    }

    respond(str) {
        this.props.frontendApi.sendStr(str, (result) => {
            
        });

        console.log("Responded " + str)
    }

    render() {
        return(
            <div className='main'>
                <div className='buttons'>
                    <Button caption="Yes" onClick={()=>this.respond("yes")}/>
                    <Button caption="No" onClick={()=>this.respond("no")}/>
                </div>

                <div className='input'>
                    <InputField/>
                </div>
            </div>
        )
    }
}